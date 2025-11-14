#include "class_parser.h"

#include "../ast/method_declaration_ast_node.h"
#include "../ast/type_declaration/class_declaration_ast_node.h"
#include "../util.h"
#include "type_parser.h"

namespace ast = codesh::ast;
namespace parser = codesh::parser;

static void parse_class_scope(std::queue<std::unique_ptr<codesh::token>> &tokens,
        ast::type_decl::class_declaration_ast_node *class_node);

static void parse_method_scope(std::queue<std::unique_ptr<codesh::token>> &tokens,
        ast::method_declaration_ast_node *method_node);


std::unique_ptr<ast::type_decl::class_declaration_ast_node> codesh::parser::parse_class_declaration(
        std::queue<std::unique_ptr<token>> &tokens)
{
    if (util::consume_token(tokens)->get_group() != token_group::KEYWORD_NAME)
    {
        throw std::runtime_error("Unexpected token: Expected ושמו");
    }


    // Get name
    const std::unique_ptr<token> name_token = util::consume_token(tokens);

    if (name_token->get_group() != token_group::IDENTIFIER)
    {
        throw std::runtime_error("Unexpected token: Expected identifier");
    }


    std::unique_ptr<ast::type_decl::class_declaration_ast_node> node = std::make_unique<ast::type_decl::class_declaration_ast_node>(
        static_cast<const identifier_token *>(name_token.get())->get_content() // NOLINT(*-pro-type-static-cast-downcast)
    );


    // Get attributes
    node->set_attributes(parse_modifiers(tokens));


    // Start scope
    if (util::consume_token(tokens)->get_group() != token_group::SCOPE_BEGIN)
    {
        throw std::runtime_error("Unexpected token: Expected start of scope (ויאמר:)");
    }

    parse_class_scope(tokens, node.get());


    return node;
}


static void parse_class_scope(std::queue<std::unique_ptr<codesh::token>> &tokens,
        ast::type_decl::class_declaration_ast_node *const class_node)
{
    while (!tokens.empty())
    {
        switch (parser::util::consume_token(tokens)->get_group())
        {
        case codesh::token_group::KEYWORD_LET:
            ast::method_declaration_ast_node *method_node; // TODO: initialize
            parse_method_scope(tokens, method_node);
            break;

        case codesh::token_group::SCOPE_END: return;
        default: throw std::runtime_error("Unexpected token");
        }
    }

    throw std::runtime_error("Unexpected EOF: Expected end of scope (ויתם:)");
}

static void parse_method_scope(std::queue<std::unique_ptr<codesh::token>> &tokens,
        ast::method_declaration_ast_node *method_node)
{
    while (!tokens.empty())
    {
        if (parser::util::consume_token(tokens)->get_group() != codesh::token_group::KEYWORD_FUNCTION) // TODO: it can be ויהי עצם another time or ויהי משתנה
        {
            throw std::runtime_error("Unexpected token: Expected מעשה");
        }

        if (parser::util::consume_token(tokens)->get_group() != codesh::token_group::KEYWORD_NAME)
        {
            throw std::runtime_error("Unexpected token: Expected ושמו");
        }
    }
}