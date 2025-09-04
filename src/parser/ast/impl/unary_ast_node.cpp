#include "unary_ast_node.h"

template <typename T> codesh::ast::impl::unary_ast_node<T>::unary_ast_node(std::unique_ptr<typed_ast_node<T>> child) :
    child(std::move(child))
{
}

template <typename T> codesh::ast::impl::typed_ast_node<T> *codesh::ast::impl::unary_ast_node<T>::get_child() const
{
    return child;
}