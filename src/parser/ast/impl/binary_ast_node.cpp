#include "binary_ast_node.h"

codesh::ast::impl::binary_ast_node::binary_ast_node(std::unique_ptr<value_ast_node> left,
                                                       std::unique_ptr<value_ast_node> right) :
    left(std::move(left)), right(std::move(right))
{
}

codesh::ast::var_reference::value_ast_node *codesh::ast::impl::binary_ast_node::get_left() const
{
    return this->left.get();
}

codesh::ast::var_reference::value_ast_node *codesh::ast::impl::binary_ast_node::get_right() const
{
    return this->right.get();
}

codesh::ast::type::type_ast_node *codesh::ast::impl::binary_ast_node::get_type() const
{
    return this->type.get();
}
