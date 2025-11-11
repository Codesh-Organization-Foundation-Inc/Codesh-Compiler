#include "modulu_operator_ast_node.h"

codesh::ast::op::modulu_operator_ast_node::modulu_operator_ast_node(std::unique_ptr<ir_convertable_ast_node> left,
                                                                           std::unique_ptr<ir_convertable_ast_node> right) :
    binary_ast_node(std::move(left), std::move(right))
{
}

void *codesh::ast::op::modulu_operator_ast_node::to_ir() const
{
    return nullptr;
}