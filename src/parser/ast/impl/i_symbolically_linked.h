#pragma once

#include <optional>

namespace codesh::ast::impl
{

/**
 * @tparam T The symbol type. Must extend {@link i_ast_produced}.
 */
template <typename T>
class i_symbolically_linked
{
protected:
    [[nodiscard]] const virtual std::optional<std::reference_wrapper<T>> &_get_resolved() const = 0;

public:
    virtual ~i_symbolically_linked();

    virtual void set_resolved(T &symbol) = 0;
    [[nodiscard]] T &get_resolved() const;
};

}

#include "parser/ast/impl/i_symbolically_linked.tpp"
