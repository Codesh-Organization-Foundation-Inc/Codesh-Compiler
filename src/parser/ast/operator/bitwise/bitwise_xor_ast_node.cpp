#include "bitwise_xor_ast_node.h"

#include "lexer/trie/keywords.h"

codesh::ast::op::bitwise_xor_ast_node::bitwise_xor_ast_node(const lexer::code_position code_position,
                                                            std::unique_ptr<value_ast_node> left,
                                                            std::unique_ptr<value_ast_node> right) :
    binary_ast_node(code_position, std::move(left), std::move(right))
{
}

std::string codesh::ast::op::bitwise_xor_ast_node::to_pretty_string() const
{
    return lexer::trie::token_to_string(token_group::OPERATOR_BITWISE_XOR);
}
