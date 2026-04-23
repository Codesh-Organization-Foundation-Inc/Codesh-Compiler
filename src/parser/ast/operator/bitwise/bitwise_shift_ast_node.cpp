#include "bitwise_shift_ast_node.h"

#include "lexer/trie/keywords.h"

#include <fmt/xchar.h>

codesh::ast::op::bitwise_shift_ast_node::bitwise_shift_ast_node(const lexer::code_position code_position,
                                                                std::unique_ptr<value_ast_node> left,
                                                                std::unique_ptr<value_ast_node> right,
                                                                const shift_direction _shift_direction) :
    binary_ast_node(code_position, std::move(left), std::move(right)), _shift_direction(_shift_direction)
{
}

std::string codesh::ast::op::bitwise_shift_ast_node::to_pretty_string() const
{
    return fmt::format(
        "{} {}",
        lexer::trie::token_to_string(token_group::OPERATOR_BITWISE_SHIFT),
        _shift_direction == shift_direction::LEFT
            ? lexer::trie::token_to_string(token_group::KEYWORD_BITWISE_LEFT)
            : lexer::trie::token_to_string(token_group::KEYWORD_BITWISE_RIGHT)
    );
}

codesh::output::ir::operator_type codesh::ast::op::bitwise_shift_ast_node::get_ir_operator_type() const
{
    return _shift_direction == shift_direction::LEFT
        ? output::ir::operator_type::SHL
        : output::ir::operator_type::SHR;
}
