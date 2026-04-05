#pragma once

#include "parser/ast/impl/binary_ast_node.h"

namespace codesh::ast::op
{

enum class shift_direction
{
    RIGHT,
    LEFT
};

class bitwise_shift_ast_node final : public impl::binary_ast_node
{
    shift_direction _shift_direction;

public:
    bitwise_shift_ast_node(lexer::code_position code_position, std::unique_ptr<value_ast_node> left,
            std::unique_ptr<value_ast_node> right, shift_direction _shift_direction);

    [[nodiscard]] std::string to_pretty_string() const override;
};

}
