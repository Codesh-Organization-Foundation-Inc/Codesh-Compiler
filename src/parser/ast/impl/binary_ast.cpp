#include "binary_ast.h"

codesh::ast::impl::ast_node &codesh::ast::impl::binary_ast::get_left() const
{
    return *this->left;
}

codesh::ast::impl::ast_node &codesh::ast::impl::binary_ast::get_right() const
{
    return *this->right;
}