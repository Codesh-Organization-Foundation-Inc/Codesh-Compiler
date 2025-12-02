#include "not_operator_ast_node.h"

codesh::ast::op::not_operator_ast_node::not_operator_ast_node(std::unique_ptr<ir_convertable_ast_node> child)
    : unary_ast_node(std::move(child))
{
}

void codesh::ast::op::not_operator_ast_node::emit_ir(output::ir::code_block &containing_block) const
{
}

