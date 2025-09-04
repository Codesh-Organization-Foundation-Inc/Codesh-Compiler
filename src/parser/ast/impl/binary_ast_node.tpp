#pragma once

#include "binary_ast_node.h"

template <typename T>
codesh::ast::impl::binary_ast_node<T>::binary_ast_node(std::unique_ptr<typed_ast_node<T>> left,
                                                       std::unique_ptr<typed_ast_node<T>> right) :
    left(std::move(left)), right(std::move(right))
{
}
template <typename T> codesh::ast::impl::typed_ast_node<T> *codesh::ast::impl::binary_ast_node<T>::get_left() const
{
    return this->left;
}

template <typename T> codesh::ast::impl::typed_ast_node<T> *codesh::ast::impl::binary_ast_node<T>::get_right() const
{
    return this->right;
}
template <typename T> void codesh::ast::impl::binary_ast_node<T>::set_left(typed_ast_node<T> &node)
{
    this->left = &node;
}
template <typename T> void codesh::ast::impl::binary_ast_node<T>::set_right(typed_ast_node<T> &node)
{
    this->right = &node;
}
