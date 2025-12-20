#include "super_call_ast_node.h"

#include "../../../../output/ir/code_block.h"
#include "../../type_declaration/class_declaration_ast_node.h"

void codesh::ast::method::operation::super_call_ast_node::emit_ir(
        output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
        const type_decl::type_declaration_ast_node &containing_type_decl) const
{
    // Load "this" for the super constructor
    containing_block.add_instruction(std::make_unique<output::ir::load_instruction>(
        output::ir::instruction_type::REFERENCE,
        0
    ));


    const auto &cp = containing_type_decl.get_constant_pool();

    const int super_constructor_cp_index = cp.get_methodref_index(
        cp.get_class_index(
            cp.get_utf8_index(
                containing_type_decl.get_super_class()->get_resolved_name().join()
            )
        ),

        cp.get_name_and_type_index(
            cp.get_utf8_index("<init>"),
            //TODO: Match parameters list
            cp.get_utf8_index("()V")
        )
    );

    containing_block.add_instruction(std::make_unique<output::ir::invoke_instruction>(
        output::ir::invokation_type::SPECIAL,
        super_constructor_cp_index
    ));
}
