#include "increment_operator_ast_node.h"

#include "lexer/trie/keywords.h"

codesh::ast::op::assignment::increment_operator_ast_node::increment_operator_ast_node(
        const lexer::code_position code_position,
        std::unique_ptr<var_reference::variable_reference_ast_node> variable) :
    increment_decrement_operator_ast_node(code_position, std::move(variable))
{
}

std::string codesh::ast::op::assignment::increment_operator_ast_node::to_pretty_string() const
{
    return lexer::trie::token_to_string(token_group::OPERATOR_INCREMENT);
}

codesh::output::ir::operator_type codesh::ast::op::assignment::increment_operator_ast_node::get_operator_type() const
{
    return output::ir::operator_type::ADD;
}
