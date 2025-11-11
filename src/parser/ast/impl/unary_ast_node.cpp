#include "unary_ast_node.h"

codesh::ast::impl::unary_ast_node::unary_ast_node(std::unique_ptr<ir_convertable_ast_node> child) :
    child(std::move(child))
{
}

codesh::ast::impl::ir_convertable_ast_node *codesh::ast::impl::unary_ast_node::get_child() const
{
    return child.get();
}