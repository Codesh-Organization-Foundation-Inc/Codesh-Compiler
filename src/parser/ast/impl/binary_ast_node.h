#pragma once

#include "parser/ast/var_reference/value_ast_node.h"
#include "defenition/primitive_type.h"
#include "operator_ast_node.h"

#include <cstddef>
#include <memory>

namespace codesh::ast::impl
{

class binary_ast_node : public operator_ast_node<2>
{
    void wrap_child_in_widening_cast(std::size_t index, definition::primitive_type target);

protected:
    binary_ast_node(blasphemy::code_position code_position, std::unique_ptr<value_ast_node> left,
            std::unique_ptr<value_ast_node> right);

public:
    [[nodiscard]] virtual value_ast_node &get_left() const;
    [[nodiscard]] value_ast_node &get_right() const;

    //TODO: Nodes like ++ should specify that only numerical types are allowed.
    [[nodiscard]] bool is_value_valid() const override;

    /**
     * Wraps the narrower child in a widening_cast_ast_node if the two types differ
     * but are widen-compatible. Returns true if types are now compatible (or already were).
     *
     * Call only after both children are resolved!
     */
    bool apply_widening_conversions();

    [[nodiscard]] type::type_ast_node *get_type() const override;
};

}
