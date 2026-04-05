#include "bitwise_shift_ast_node.h"

#include "lexer/trie/keywords.h"

#include <fmt/xchar.h>

codesh::ast::op::bitwise_shift_ast_node::bitwise_shift_ast_node(const lexer::code_position code_position,
                                                                std::unique_ptr<value_ast_node> left,
                                                                std::unique_ptr<value_ast_node> right,
                                                                const direction shift_direction) :
    binary_ast_node(code_position, std::move(left), std::move(right)), shift_direction(shift_direction)
{
}

std::string codesh::ast::op::bitwise_shift_ast_node::to_pretty_string() const
{
    return fmt::format(
        "{} {}",
        lexer::trie::token_to_string(token_group::OPERATOR_BITWISE_SHIFT),
        shift_direction == direction::LEFT
            ? lexer::trie::token_to_string(token_group::KEYWORD_BITWISE_LEFT)
            : lexer::trie::token_to_string(token_group::KEYWORD_BITWISE_RIGHT)
    );
}