#include "equals_operator_ast_node.h"

template <typename T>
codesh::ast::op::equals_operator_ast_node<T>::equals_operator_ast_node(std::unique_ptr<impl::typed_ast_node<T>> left,
                                                                           std::unique_ptr<impl::typed_ast_node<T>> right) :
    impl::binary_ast_node<T>(std::move(left), std::move(right))
{
}

template <typename T> T codesh::ast::op::equals_operator_ast_node<T>::evaluate() const
{
    return this->get_left() == this->get_right();
}