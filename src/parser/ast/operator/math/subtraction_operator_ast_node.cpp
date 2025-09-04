#include "subtraction_operator_ast_node.h"

#include "../../node_type.h"

template <typename T>
codesh::ast::op::subtraction_operator_ast_node<T>::subtraction_operator_ast_node(std::unique_ptr<impl::typed_ast_node<T>> left,
                                                                           std::unique_ptr<impl::typed_ast_node<T>> right) :
    impl::binary_ast_node<T>(std::move(left), std::move(right))
{
}

template <typename T> codesh::ast::node_type codesh::ast::op::subtraction_operator_ast_node<T>::get_type() const
{
    return node_type::SUBTRACTION;
}

template <typename T> T codesh::ast::op::subtraction_operator_ast_node<T>::evaluate() const
{
    return this->get_left() - this->get_right();
}