#include "division_operator_ast_node.h"

#include "lexer/trie/keywords.h"

codesh::ast::op::division_operator_ast_node::division_operator_ast_node(const blasphemy::code_position code_position,
        std::unique_ptr<value_ast_node> left, std::unique_ptr<value_ast_node> right) :
    binary_ast_node(code_position, std::move(left), std::move(right))
{
}

std::string codesh::ast::op::division_operator_ast_node::to_pretty_string() const
{
    return lexer::trie::TOKEN_TO_NAME_MAP.at(token_group::OPERATOR_DIVISION);
}

codesh::output::ir::operator_type codesh::ast::op::division_operator_ast_node::get_ir_operator_type() const
{
    return output::ir::operator_type::DIV;
}
