#include "greater_equals_operator_ast_node.h"

codesh::ast::op::greater_equals_operator_ast_node::greater_equals_operator_ast_node(std::unique_ptr<i_ir_emitter> left,
                                                                           std::unique_ptr<i_ir_emitter> right) :
    binary_ast_node(std::move(left), std::move(right))
{
}

void codesh::ast::op::greater_equals_operator_ast_node::emit_ir(
    output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
    const type_decl::type_declaration_ast_node &containing_type_decl) const
{
}