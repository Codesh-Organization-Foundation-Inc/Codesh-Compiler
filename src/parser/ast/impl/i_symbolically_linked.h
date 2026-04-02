#pragma once

#include <optional>

namespace codesh::ast::impl
{

class i_symbolically_linked_base
{
public:
    virtual ~i_symbolically_linked_base();
    [[nodiscard]] virtual bool is_resolved() const = 0;
};

/**
 * @tparam T The symbol type. Must extend @link i_ast_produced \endlink.
 */
template <typename T>
class i_symbolically_linked : public i_symbolically_linked_base
{
protected:
    [[nodiscard]] const virtual std::optional<std::reference_wrapper<T>> &_get_resolved() const = 0;

public:
    virtual void set_resolved(T &symbol) = 0;
    [[nodiscard]] T &get_resolved() const;
    [[nodiscard]] bool is_resolved() const override;
};

}

#include "i_symbolically_linked.tpp"
