#pragma once

#include "defenition/fully_qualified_name.h"
#include "i_symbolically_linked.h"

namespace codesh::ast::impl
{
class ast_node;
}

namespace codesh::ast::impl
{

/**
 * @tparam T The symbol type. Must extend @link i_resolvable_symbol \endlink.
 */
template <typename T>
class i_resolvable : public i_symbolically_linked<T>
{
public:
    /**
     * @returns The unresolved type name
     */
    [[nodiscard]] virtual const definition::fully_qualified_name &get_unresolved_name() const = 0;
    /**
     * @returns The resolved type name.
     * Shorthand for @c i_resolvable_symbol<T>::get_full_name
     */
    [[nodiscard]] const definition::fully_qualified_name &get_resolved_name() const;

    [[nodiscard]] lexer::code_range get_name_range() const;

    /**
     * @param resolved When true, behaves like @link get_resolved_name \endlink.
     * When false, behaves like @link get_unresolved_name \endlink
     */
    [[nodiscard]] const definition::fully_qualified_name &get_name(bool resolved) const;
    [[nodiscard]] std::string get_last_name(bool resolved) const;
};

}

#include "i_resolvable.tpp"