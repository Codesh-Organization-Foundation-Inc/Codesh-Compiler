#include "not_operator_ast_node.h"

codesh::ast::op::not_operator_ast_node::not_operator_ast_node(std::unique_ptr<method_ast_node> child)
    : unary_ast_node(std::move(child))
{
}

void *codesh::ast::op::not_operator_ast_node::to_ir() const
{
    return nullptr;
}
