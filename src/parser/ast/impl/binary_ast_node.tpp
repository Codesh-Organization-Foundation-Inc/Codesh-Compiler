#pragma once

#include "binary_ast_node.h"

template <typename T> codesh::ast::typed_ast_node<T> &codesh::ast::impl::binary_ast_node<T>::get_left() const
{
    return *this->left;
}

template <typename T> codesh::ast::typed_ast_node<T> &codesh::ast::impl::binary_ast_node<T>::get_right() const
{
    return *this->right;
}
