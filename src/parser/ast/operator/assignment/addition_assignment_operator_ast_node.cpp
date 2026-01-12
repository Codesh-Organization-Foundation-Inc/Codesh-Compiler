#include "addition_assignment_operator_ast_node.h"

codesh::ast::op::assignment::addition_assignment_operator_ast_node::addition_assignment_operator_ast_node(
    std::unique_ptr<variable_reference_ast_node> left, std::unique_ptr<value_ast_node> right):
assignment_operator_ast_node(std::move(left), std::move(right))
{
}

void codesh::ast::op::assignment::addition_assignment_operator_ast_node::emit_ir(
    output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
    const type_decl::type_declaration_ast_node &containing_type_decl) const
{
}