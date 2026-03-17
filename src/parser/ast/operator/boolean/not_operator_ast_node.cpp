#include "not_operator_ast_node.h"

#include "lexer/trie/keywords.h"

codesh::ast::op::not_operator_ast_node::not_operator_ast_node(const lexer::code_position code_position,
        std::unique_ptr<value_ast_node> child) :
    unary_ast_node(code_position, std::move(child))
{
}

std::string codesh::ast::op::not_operator_ast_node::to_pretty_string() const
{
    return lexer::trie::TOKEN_TO_NAME_MAP.at(token_group::OPERATOR_NOT);
}
