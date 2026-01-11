#include "assign_operator_ast_node.h"

#include "../../../../output/ir/code_block.h"
#include "../../../../semantic_analyzer/symbol_table/symbol.h"

codesh::ast::op::assignment::assign_operator_ast_node::assign_operator_ast_node(
        std::unique_ptr<variable_reference_ast_node> left, std::unique_ptr<value_ast_node> right) :
    assignment_operator_ast_node(std::move(left), std::move(right))
{
}

void codesh::ast::op::assignment::assign_operator_ast_node::emit_ir(
    output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
    const type_decl::type_declaration_ast_node &containing_type_decl) const
{
    get_right().emit_ir(containing_block, symbol_table, containing_type_decl);

    const auto &variable_symbol = get_left().get_resolved();
    if (const auto &local_var = dynamic_cast<const semantic_analyzer::local_variable_symbol *>(&variable_symbol))
    {
        containing_block.add_instruction(std::make_unique<output::ir::store_in_local_var_instruction>(
            local_var->get_type()->to_instruction_type(),
            local_var->get_index()
        ));
    }
    else
    {
        throw std::runtime_error("Variable type not yet supported");
    }
}
