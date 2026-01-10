#pragma once

#include "defenition/fully_qualified_class_name.h"
#include "parser/ast/impl/i_symbolically_linked.h"

namespace codesh::ast::impl
{
class ast_node;
}

namespace codesh::ast::impl
{

/**
 * @tparam T The symbol type. Must extend {@link i_resolvable_symbol}.
 */
template <typename T>
class i_resolvable : public i_symbolically_linked<T>
{
public:
    /**
     * @returns The unresolved type name
     */
    [[nodiscard]] virtual const definition::fully_qualified_class_name &get_unresolved_name() const = 0;
    /**
     * @returns The resolved type name.
     * Shorthand for {@code {@link i_resolvable_symbol<T>::get_full_name}}
     */
    [[nodiscard]] const definition::fully_qualified_class_name &get_resolved_name() const;

    /**
     * @param resolved When true, behaves like {@link get_resolved_name}.
     * When false, behaves like {@link get_unresolved_name}
     */
    [[nodiscard]] const definition::fully_qualified_class_name &get_name(bool resolved) const;
    [[nodiscard]] std::string get_last_name(bool resolved) const;
};

}

#include "parser/ast/impl/i_resolvable.tpp"