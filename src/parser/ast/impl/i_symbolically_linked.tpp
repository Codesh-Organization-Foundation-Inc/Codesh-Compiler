#pragma once

template <typename T>
T &codesh::ast::impl::i_symbolically_linked<T>::get_resolved() const
{
    return _get_resolved().value();
}

template <typename T>
bool codesh::ast::impl::i_symbolically_linked<T>::is_resolved() const
{
    return _get_resolved().has_value();
}