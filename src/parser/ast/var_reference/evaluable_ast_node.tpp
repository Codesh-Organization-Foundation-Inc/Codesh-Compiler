#pragma once

template <typename T>
codesh::ast::var_reference::evaluable_ast_node<T>::evaluable_ast_node(std::unique_ptr<type::type_ast_node> type, T value)
    : value_ast_node(std::move(type)), value(std::move(value))
{
}

template <typename T>
T codesh::ast::var_reference::evaluable_ast_node<T>::get_value() const
{
    return value;
}

template <typename T>
void codesh::ast::var_reference::evaluable_ast_node<T>::set_value(T value)
{
    this->value = std::move(value);
}
