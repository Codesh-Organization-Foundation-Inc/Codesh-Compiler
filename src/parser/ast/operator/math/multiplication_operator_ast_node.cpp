#include "multiplication_operator_ast_node.h"

#include "lexer/trie/keywords.h"

codesh::ast::op::multiplication_operator_ast_node::multiplication_operator_ast_node(
    std::unique_ptr<value_ast_node> left,
        std::unique_ptr<value_ast_node> right
) :
    binary_ast_node(std::move(left), std::move(right))
{
}

std::string codesh::ast::op::multiplication_operator_ast_node::to_pretty_string() const
{
    return lexer::trie::TOKEN_TO_NAME_MAP.at(token_group::OPERATOR_MULTIPLICATION);
}

void codesh::ast::op::multiplication_operator_ast_node::emit_ir(
    output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
    const type_decl::type_declaration_ast_node &containing_type_decl) const
{
}