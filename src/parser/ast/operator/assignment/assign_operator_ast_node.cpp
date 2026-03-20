#include "assign_operator_ast_node.h"

#include "lexer/trie/keywords.h"
#include "parser/ast/operator/boolean/not_operator_ast_node.h"
#include "semantic_analyzer/symbol_table/symbol.h"

codesh::ast::op::assignment::assign_operator_ast_node::assign_operator_ast_node(
        const lexer::code_position code_position, std::unique_ptr<variable_reference_ast_node> left,
        std::unique_ptr<value_ast_node> right) :
    assignment_operator_ast_node(code_position, std::move(left), std::move(right))
{
}

std::string codesh::ast::op::assignment::assign_operator_ast_node::to_pretty_string() const
{
    return lexer::trie::token_to_string(token_group::KEYWORD_REPLACE);
}

codesh::output::ir::operator_type codesh::ast::op::assignment::assign_operator_ast_node::
    get_operator_type() const
{
    return output::ir::operator_type::ASSIGN;
}
