#pragma once

#include "parser/ast/impl/binary_ast_node.h"
#include "parser/ast/operator/math/i_math_operator_holder.h"

namespace codesh::ast::op
{

enum class shift_direction
{
    RIGHT,
    LEFT
};

class bitwise_shift_ast_node final : public impl::binary_ast_node, public i_math_operator_holder
{
    shift_direction _shift_direction;

public:
    bitwise_shift_ast_node(lexer::code_position code_position, std::unique_ptr<value_ast_node> left,
            std::unique_ptr<value_ast_node> right, shift_direction _shift_direction);

    [[nodiscard]] std::string to_pretty_string() const override;

    [[nodiscard]] output::ir::operator_type get_ir_operator_type() const override;
};

}
 