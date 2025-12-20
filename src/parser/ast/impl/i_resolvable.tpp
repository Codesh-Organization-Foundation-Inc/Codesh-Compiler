#pragma once

#include <iostream>


template <typename T>
codesh::ast::impl::i_resolvable<T>::~i_resolvable() = default;

template <typename T>
const codesh::definition::fully_qualified_class_name &codesh::ast::impl::i_resolvable<T>::get_resolved_name() const
{
    const std::optional<std::reference_wrapper<T>> &resolved = _get_resolved();

    if (!resolved)
    {
        //TODO: Throw:
        // throw std::runtime_error("Attempted to get resolved name while none exists");

        std::cerr << "NOTICE: USING UNRESOLVED NAME " << get_unresolved_name().join() << std::endl;
        return get_unresolved_name();
    }

    return resolved.value().get().get_full_name();
}

template <typename T>
const codesh::definition::fully_qualified_class_name &codesh::ast::impl::i_resolvable<T>::get_name(
    const bool resolved) const
{
    if (resolved)
        return get_resolved_name();

    return get_unresolved_name();
}

template <typename T>
T &codesh::ast::impl::i_resolvable<T>::get_resolved() const
{
    return _get_resolved().value();
}
