#include "type_parser.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "defenition/visibility.h"
#include "parser/ast/type_declaration/class_declaration_ast_node.h"
#include "parser/util.h"
#include "parser/type/class/class_parser.h"

namespace ast = codesh::ast;


std::unique_ptr<ast::type_decl::type_declaration_ast_node> codesh::parser::parse_type_declaration(
        std::queue<std::unique_ptr<token>> &tokens)
{
    const auto declaration_pos = tokens.front()->get_code_position();
    tokens.pop();

    switch (util::consume_token(tokens, blasphemy::details::UNEXPECTED_DECLARATION)->get_group())
    {
    case token_group::KEYWORD_CLASS: return parse_class_declaration(declaration_pos, tokens);
    case token_group::KEYWORD_INTERFACE:; //TODO
    case token_group::KEYWORD_ENUM:; //TODO
    case token_group::KEYWORD_ANNOTATION: return nullptr; //TODO

    default: throw std::runtime_error(blasphemy::details::UNEXPECTED_DECLARATION);
    }
}


std::unique_ptr<ast::type_decl::attributes_ast_node> codesh::parser::parse_modifiers(
        blasphemy::code_position code_position, std::queue<std::unique_ptr<token>> &tokens)
{
    std::unique_ptr<ast::type_decl::attributes_ast_node> node = std::make_unique<ast::type_decl::attributes_ast_node>(
        code_position
    );

    // Attributes are optional, so check whether they exist at all.
    bool attributes_exist = false;


    // Optional 1: Static
    if (util::consuming_check(tokens, token_group::KEYWORD_STATIC))
    {
        node->set_is_static(true);
        attributes_exist = true;
    }

    // Optional 2: Visibility
    if (const auto visibility = definition::token_group_to_visibility(tokens.front().get()))
    {
        node->set_visibility(visibility.value());
        tokens.pop();
        attributes_exist = true;
    }

    // Optional 3: Abstract
    if (util::consuming_check(tokens, token_group::KEYWORD_ABSTRACT))
    {
        node->set_is_abstract(true);
        attributes_exist = true;
    }

    // Optional 4: Final
    if (util::consuming_check(tokens, token_group::KEYWORD_FINAL))
    {
        node->set_is_final(true);
        attributes_exist = true;
    }


    if (!attributes_exist)
        return node;

    // If the last keyword wasn't Shall Be, it means that the user entered a nonsensical keyword before,
    // or did not close the attribute statement with Shall Be.
    if (!util::consuming_check(tokens, token_group::KEYWORD_SHALL_BE))
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(blasphemy::details::NO_KEYWORD_SHALL_BE,
            blasphemy::blasphemy_type::SYNTAX);
    }

    return node;
}