#pragma once

#include "../../../defenition/fully_qualified_class_name.h"

#include <optional>

namespace codesh::ast::impl
{
class ast_node;
}

namespace codesh::ast::impl
{

/**
 * @tparam T The symbol type. Must extend {@link i_resolveable_symbol}.
 */
template <typename T>
class i_resolvable
{
protected:
    [[nodiscard]] const virtual std::optional<std::reference_wrapper<T>> &_get_resolved() const = 0;

public:
    virtual ~i_resolvable();

    /**
     * @returns The unresolved type name
     */
    [[nodiscard]] virtual const definition::fully_qualified_class_name &get_unresolved_name() const = 0;
    /**
     * @returns The resolved type name.
     * Shorthand for {@code {@link i_resolveable_symbol<T>::get_full_name}}
     */
    [[nodiscard]] const definition::fully_qualified_class_name &get_resolved_name() const;

    /**
     * @param resolved When true, behaves like {@link get_resolved_name}.
     * When false, behaves like {@link get_unresolved_name}
     */
    [[nodiscard]] const definition::fully_qualified_class_name &get_name(bool resolved) const;
    [[nodiscard]] std::string get_last_name(bool resolved) const;

    virtual void set_resolved(T &symbol) = 0;
    [[nodiscard]] T &get_resolved() const;
};

}

#include "i_resolvable.tpp"