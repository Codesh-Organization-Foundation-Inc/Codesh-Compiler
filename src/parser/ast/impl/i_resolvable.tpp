#pragma once

// ReSharper disable once CppUnusedIncludeDirective
#include "semantic_analyzer/symbol_table/symbol.h"

//NOTE: Circular dependency with definition.h, so just write it directly.
static const codesh::definition::fully_qualified_name ERROR_FQN = "סְדוֹם וַעֲמֹרָה";

template <typename T>
const codesh::definition::fully_qualified_name &codesh::ast::impl::i_resolvable<T>::get_resolved_name() const
{
    const std::optional<std::reference_wrapper<T>> &resolved = this->_get_resolved();

    if (!resolved.has_value())
        return ERROR_FQN;

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
