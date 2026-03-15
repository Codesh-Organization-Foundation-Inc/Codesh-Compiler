#pragma once

// ReSharper disable once CppUnusedIncludeDirective
#include "semantic_analyzer/symbol_table/symbol.h"

#include <cassert>

template <typename T>
const codesh::definition::fully_qualified_name &codesh::ast::impl::i_resolvable<T>::get_resolved_name() const
{
    const std::optional<std::reference_wrapper<T>> &resolved = this->_get_resolved();
    assert(resolved.has_value() && "Attempted to get resolved name while none exists");

    return resolved.value().get().get_full_name();
}

template <typename T>
const codesh::definition::fully_qualified_name &codesh::ast::impl::i_resolvable<T>::get_name(
    const bool resolved) const
{
    if (resolved)
        return get_resolved_name();

    return get_unresolved_name();
}

template <typename T>
std::string codesh::ast::impl::i_resolvable<T>::get_last_name(const bool resolved) const
{
    return (resolved ? get_resolved_name() : get_unresolved_name()).get_last_part();
}
