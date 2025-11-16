#include "type_parser.h"

#include "../../defenition/visibility.h"
#include "../ast/type_declaration/class_declaration_ast_node.h"
#include "../util.h"
#include "class/class_parser.h"

namespace ast = codesh::ast;


std::unique_ptr<ast::type_decl::type_declaration_ast_node> codesh::parser::parse_type_declaration(
        std::queue<std::unique_ptr<token>> &tokens)
{
    tokens.pop();

    switch (util::consume_token(tokens)->get_group())
    {
    case token_group::KEYWORD_CLASS: return parse_class_declaration(tokens);
    case token_group::KEYWORD_INTERFACE:; //TODO
    case token_group::KEYWORD_ENUM:; //TODO
    case token_group::KEYWORD_ANNOTATION: return nullptr; //TODO

    default: throw std::runtime_error("Unexpected token: Expected class, interface, enum or annotation");
    }
}


std::unique_ptr<ast::type_decl::attributes_ast_node> codesh::parser::parse_modifiers(
        std::queue<std::unique_ptr<token>> &tokens)
{
    std::unique_ptr<ast::type_decl::attributes_ast_node> node = std::make_unique<ast::type_decl::attributes_ast_node>();

    // Attributes are optional, so check whether they exist at all.
    if (tokens.empty() || tokens.front()->get_group() == token_group::SCOPE_BEGIN)
        return node;


    // Optional 1: Static
    if (util::consuming_check(tokens, token_group::KEYWORD_STATIC))
    {
        node->set_is_static(true);
    }

    // Optional 2: Visibility
    if (const auto visibility = definition::token_group_to_visibility(tokens.front().get()))
    {
        node->set_visibility(visibility.value());
        tokens.pop();
    }

    // Optional 3: Abstract
    if (util::consuming_check(tokens, token_group::KEYWORD_ABSTRACT))
    {
        node->set_is_abstract(true);
    }

    // Optional 4: Final
    if (util::consuming_check(tokens, token_group::KEYWORD_FINAL))
    {
        node->set_is_final(true);
    }


    if (!util::consuming_check(tokens, token_group::KEYWORD_SHALL_BE))
    {
        // If the last keyword wasn't Shall Be, it means that the user entered a nonsensical keyword before,
        // or did not close the attribute statement with Shall Be.
        throw std::runtime_error("Unexpected token: Expected attribute list enclosed by היה");
    }

    return node;
}