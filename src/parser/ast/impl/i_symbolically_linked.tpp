#pragma once

template <typename T>
codesh::ast::impl::i_symbolically_linked<T>::~i_symbolically_linked() = default;

template <typename T>
T &codesh::ast::impl::i_symbolically_linked<T>::get_resolved() const
{
    return _get_resolved().value();
}