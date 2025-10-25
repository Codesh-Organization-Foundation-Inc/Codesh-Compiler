#include "not_operator_ast_node.h"

template <typename T>
codesh::ast::op::not_operator_ast_node<T>::not_operator_ast_node(std::unique_ptr<impl::typed_ast_node<T>> child)
    : impl::unary_ast_node<T>(child)
{
}

template <typename T> T codesh::ast::op::not_operator_ast_node<T>::evaluate() const
{
    return !this->child->evaluate();
}