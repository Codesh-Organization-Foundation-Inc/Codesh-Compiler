#include "binary_ast_node.h"

codesh::ast::impl::ast_node &codesh::ast::impl::binary_ast_node::get_left() const
{
    return *this->left;
}

codesh::ast::impl::ast_node &codesh::ast::impl::binary_ast_node::get_right() const
{
    return *this->right;
}