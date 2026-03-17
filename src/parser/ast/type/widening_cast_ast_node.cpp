#include "widening_cast_ast_node.h"

#include "output/ir/code_block.h"
#include "output/ir/instruction/widening_cast_instruction.h"
#include "parser/ast/impl/i_constant_pool_emitter.h"
#include "parser/ast/method/operation/method_operation_ast_node.h"

codesh::ast::type::widening_cast_ast_node::widening_cast_ast_node(
        const lexer::code_position code_position, std::unique_ptr<value_ast_node> inner,
        std::unique_ptr<primitive_type_ast_node> target_type) :
    value_ast_node(code_position),
    child(std::move(inner)),
    target_type(std::move(target_type))
{
}

codesh::ast::type::type_ast_node *codesh::ast::type::widening_cast_ast_node::get_type() const
{
    return target_type.get();
}

void codesh::ast::type::widening_cast_ast_node::set_statement_index(const size_t statement_index)
{
    method_operation_ast_node::set_statement_index(statement_index);
    child->set_statement_index(statement_index);
}

void codesh::ast::type::widening_cast_ast_node::emit_constants(
        const compilation_unit_ast_node &root_node,
        output::jvm_target::constant_pool &constant_pool)
{
    if (const auto cp_emitter = dynamic_cast<i_constant_pool_emitter *>(child.get()))
    {
        cp_emitter->emit_constants(root_node, constant_pool);
    }
}

void codesh::ast::type::widening_cast_ast_node::emit_ir(
        output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
        const ast::type_decl::type_declaration_ast_node &containing_type_decl) const
{
    child->emit_ir(containing_block, symbol_table, containing_type_decl);

    containing_block.add_instruction(
        std::make_unique<output::ir::widening_cast_instruction>(
            child->get_type()->to_instruction_type(),
            target_type->to_instruction_type()
        )
    );
}
