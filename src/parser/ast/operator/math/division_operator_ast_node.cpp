#include "division_operator_ast_node.h"

#include "../../node_type.h"

template <typename T>
codesh::ast::op::division_operator_ast_node<T>::division_operator_ast_node(std::unique_ptr<typed_ast_node<T>> left,
                                                                           std::unique_ptr<typed_ast_node<T>> right) :
    impl::binary_ast_node<T>(std::move(left), std::move(right))
{
}

template <typename T> codesh::ast::node_type codesh::ast::op::division_operator_ast_node<T>::get_type() const
{
    return node_type::DIVISION;
}

template <typename T> T codesh::ast::op::division_operator_ast_node<T>::evaluate() const
{
    return this->get_left() / this->get_right();
}