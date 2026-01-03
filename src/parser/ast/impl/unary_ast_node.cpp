#include "unary_ast_node.h"

codesh::ast::impl::unary_ast_node::unary_ast_node(std::unique_ptr<value_ast_node> child) :
    child(std::move(child))
{
}

codesh::ast::var_reference::value_ast_node *codesh::ast::impl::unary_ast_node::get_child() const
{
    return child.get();
}

codesh::ast::type::type_ast_node *codesh::ast::impl::unary_ast_node::get_type() const
{
    return this->type.get();
}