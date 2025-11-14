#include "class_parser.h"

#include "../ast/method_declaration_ast_node.h"
#include "../ast/type_declaration/class_declaration_ast_node.h"
#include "../util.h"
#include "type_parser.h"

namespace ast = codesh::ast;
namespace parser = codesh::parser;

static void parse_class_scope(std::queue<std::unique_ptr<codesh::token>> &tokens,
        ast::type_decl::class_declaration_ast_node *class_node);

static void parse_method_scope(std::queue<std::unique_ptr<codesh::token>> &tokens);


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
            {
            codesh::token_group kind = tokens.front()->get_group();
            if (kind == codesh::token_group::KEYWORD_FUNCTION)
            {
                parse_method_scope(tokens);
            }
            else if (kind == codesh::token_group::KEYWORD_CLASS)
            {
                // TODO: Add ויהי עצם
                continue;
            }
            else if (kind == codesh::token_group::KEYWORD_VAR)
            {
                //TODO: Add ויהי משתנה
                continue;
            }
            else
            {
                throw std::runtime_error("Unexpected token"); //TODO: Add custom codesh error messege
            }
            break;
            }
        case codesh::token_group::SCOPE_END: return;
        default: throw std::runtime_error("Unexpected token");
        }
    }
    throw std::runtime_error("Unexpected EOF: Expected end of scope (ויתם:)");
}

static void parse_method_scope(std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    while (!tokens.empty())
    {
        // ושמו
        if (parser::util::consume_token(tokens)->get_group() != codesh::token_group::KEYWORD_NAME)
        {
            throw std::runtime_error("Unexpected token: Expected ושמו");
        }


        const std::unique_ptr<codesh::token> name_token = parser::util::consume_token(tokens);
        if (name_token->get_group() != codesh::token_group::IDENTIFIER)
        {
            throw std::runtime_error("Unexpected token: Expected identifier");
        }

        ast::method_declaration_ast_node method_node;
        method_node.set_name(dynamic_cast<const codesh::identifier_token *>(name_token.get())->get_content());

    }
}