#include "new_ast_node.h"

#include "defenition/definitions.h"
#include "output/ir/code_block.h"
#include "output/ir/instruction/dup_instruction.h"
#include "output/ir/instruction/invoke_instruction.h"
#include "output/ir/instruction/new_instruction.h"
#include "output/ir/instruction/pop_instruction.h"
#include "parser/ast/type_declaration/type_declaration_ast_node.h"
#include "semantic_analyzer/symbol_table/symbol_table.h"

codesh::ast::op::new_ast_node::new_ast_node(const lexer::code_position code_position,
        std::unique_ptr<type::custom_type_ast_node> constructed_type) :
    method_call_ast_node(code_position), constructed_type(std::move(constructed_type))
{
    // Constructor name of new is always <init>
    get_fqn().add(definition::JVM_CONSTRUCTOR_NAME);
}

codesh::ast::type::custom_type_ast_node &codesh::ast::op::new_ast_node::get_constructed_type() const
{
    return *constructed_type;
}

codesh::ast::type::type_ast_node *codesh::ast::op::new_ast_node::get_type() const
{
    return constructed_type.get();
}

std::string codesh::ast::op::new_ast_node::to_pretty_string() const
{
    return fmt::format(
        definition::CONSTRUCTOR_PRETTY_STRING,
        constructed_type->to_pretty_string()
    );
}

void codesh::ast::op::new_ast_node::emit_ir(output::ir::code_block &containing_block,
        const semantic_analyzer::symbol_table &symbol_table,
        const type_decl::type_declaration_ast_node &containing_type_decl) const
{
    const auto &cp = containing_type_decl.get_constant_pool();

    const auto constructed_class_cpi = cp.get_class_index(
        cp.get_utf8_index(
            constructed_type->get_resolved_name().join()
        )
    );

    // add new #class_cpi
    containing_block.add_instruction(std::make_unique<output::ir::new_instruction>(
        constructed_class_cpi
    ));

    // add dup
    containing_block.add_instruction(std::make_unique<output::ir::dup_instruction>());

    // load arguments
    for (const auto &[arg_name, arg_value] : get_arguments())
    {
        arg_value->emit_ir(containing_block, symbol_table, containing_type_decl);
    }

    // Call constructor
    const int method_cpi = cp.get_methodref_index(
        constructed_class_cpi,

        cp.get_name_and_type_index(
            cp.get_utf8_index(get_last_name(true)),
            cp.get_utf8_index(generate_descriptor())
        )
    );

    containing_block.add_instruction(std::make_unique<output::ir::invoke_instruction>(
        output::ir::invokation_type::SPECIAL,
        method_cpi,
        get_arguments().size()
    ));

    if (!containing_block.get_is_consuming())
    {
        containing_block.add_instruction(std::make_unique<output::ir::pop_instruction>());
    }
}
