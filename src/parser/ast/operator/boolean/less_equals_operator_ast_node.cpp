#include "less_equals_operator_ast_node.h"

codesh::ast::op::less_equals_operator_ast_node::less_equals_operator_ast_node(std::unique_ptr<method_ast_node> left,
                                                                           std::unique_ptr<method_ast_node> right) :
    binary_ast_node(std::move(left), std::move(right))
{
}

void *codesh::ast::op::less_equals_operator_ast_node::to_ir() const
{
    return nullptr;
}