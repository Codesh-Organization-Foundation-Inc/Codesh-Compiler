#include "not_operator_ast_node.h"

#include "lexer/trie/keywords.h"

codesh::ast::op::not_operator_ast_node::not_operator_ast_node(const blasphemy::code_position code_position,
        std::unique_ptr<value_ast_node> child) :
    unary_ast_node(code_position, std::move(child)),
    type(std::make_unique<type::primitive_type_ast_node>(
        code_position,
        definition::primitive_type::BOOLEAN
    ))
{
}

std::string codesh::ast::op::not_operator_ast_node::to_pretty_string() const
{
    return lexer::trie::TOKEN_TO_NAME_MAP.at(token_group::OPERATOR_NOT);
}

codesh::ast::type::type_ast_node *codesh::ast::op::not_operator_ast_node::get_type() const
{
    return type.get();
}

void codesh::ast::op::not_operator_ast_node::emit_ir(
    output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
    const type_decl::type_declaration_ast_node &containing_type_decl) const
{
}
