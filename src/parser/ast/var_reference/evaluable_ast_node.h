#pragma once

#include "../type/type_ast_node.h"
#include "value_ast_node.h"

namespace codesh::ast::var_reference
{

template <typename T>
class evaluable_ast_node : public value_ast_node
{
    const std::unique_ptr<type::type_ast_node> type;
    T value;

public:
    evaluable_ast_node(std::unique_ptr<type::type_ast_node> type, T value);

    [[nodiscard]] type::type_ast_node *get_type() const override;

    /**
     * @return The value.
     * @note If this is a reference, refer to the symbol table for more information on the passed type.
     */
    [[nodiscard]] T get_value() const;
    void set_value(T value);
};

}

#include "evaluable_ast_node.tpp"
