#include "class_parser.h"

#include "../ast/method_declaration_ast_node.h"
#include "../ast/type_declaration/class_declaration_ast_node.h"
#include "../ast/local_variable_declaration_ast_node.h"
#include "../util.h"
#include "../ast/type/primitive_type_ast_node.h"
#include "../ast/type/custom_type_ast_node.h"
#include "type_parser.h"

namespace ast = codesh::ast;
namespace parser = codesh::parser;

static std::unique_ptr<ast::type::type_ast_node> parse_type(std::queue<std::unique_ptr<codesh::token>> &tokens);

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
                // class_node->get_methods().push_back(std::move(...)); // TODO: Add somthing like this
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

        // * (the name)
        const std::unique_ptr<codesh::token> name_token = parser::util::consume_token(tokens);
        if (name_token->get_group() != codesh::token_group::IDENTIFIER)
        {
            throw std::runtime_error("Unexpected token: Expected identifier");
        }

        ast::method_declaration_ast_node method_node;

        method_node.set_name(dynamic_cast<const codesh::identifier_token *>(name_token.get())->get_content());

        // Get attributes
        method_node.set_attributes(parser::parse_modifiers(tokens));

        const std::unique_ptr<codesh::token> next_token = parser::util::consume_token(tokens);

        if (next_token->get_group() == codesh::token_group::KEYWORD_TAKES)
        {
            while (true)
            {
                // Parse parameter type
                std::unique_ptr<ast::type::type_ast_node> param_type = parse_type(tokens);

                // ושמו
                if (parser::util::consume_token(tokens)->get_group() != codesh::token_group::KEYWORD_NAME)
                    throw std::runtime_error("Expected ושמו");

                // * (the name)
                std::unique_ptr<codesh::token> token_name = parser::util::consume_token(tokens);
                if (token_name->get_group() != codesh::token_group::IDENTIFIER)
                    throw std::runtime_error("Expected identifier");

                auto param = std::make_unique<ast::local_variable_declaration_ast_node>();
                param->set_type(std::move(param_type));
                param->set_name(
                    dynamic_cast<codesh::identifier_token *>(token_name.get())->get_content()
                );

                method_node.get_parameter_types().push_back(std::move(param));

                // if next token is ויקח continue the loop
                if (tokens.empty() || tokens.front()->get_group() != codesh::token_group::KEYWORD_TAKES)
                    break;

                parser::util::consume_token(tokens); // consume additional ויקח
            }

            // Now read next token (either וישב or :ויאמר)
            continue;
        }


        if (next_token->get_group() == codesh::token_group::KEYWORD_RETURN)
        {
            method_node.set_return_type(parse_type(tokens));
        }
        else
        {
            // If no וישב ,return type = void
            method_node.set_return_type(
                std::make_unique<ast::type::primitive_type_ast_node>(codesh::definition::primitive_type::VOID));
        }

    }
}

static std::unique_ptr<ast::type::type_ast_node> parse_type(std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    if (tokens.empty())
    {
        throw std::runtime_error("Unexpected EOF: expected type");
    }

    auto type_token = parser::util::consume_token(tokens);
    switch (type_token->get_group())
    {
    case codesh::token_group::KEYWORD_INTEGER:
        return std::make_unique<ast::type::primitive_type_ast_node>(codesh::definition::primitive_type::INTEGER);

    case codesh::token_group::KEYWORD_FLOAT:
        return std::make_unique<ast::type::primitive_type_ast_node>(codesh::definition::primitive_type::FLOAT);

    case codesh::token_group::KEYWORD_DOUBLE:
        return std::make_unique<ast::type::primitive_type_ast_node>(codesh::definition::primitive_type::DOUBLE);

    case codesh::token_group::KEYWORD_LONG:
        return std::make_unique<ast::type::primitive_type_ast_node>(codesh::definition::primitive_type::LONG);

    case codesh::token_group::KEYWORD_SHORT:
        return std::make_unique<ast::type::primitive_type_ast_node>(codesh::definition::primitive_type::SHORT);

    case codesh::token_group::KEYWORD_BYTE:
        return std::make_unique<ast::type::primitive_type_ast_node>(codesh::definition::primitive_type::BYTE);

    case codesh::token_group::KEYWORD_CHAR:
        return std::make_unique<ast::type::primitive_type_ast_node>(codesh::definition::primitive_type::CHAR);

    case codesh::token_group::KEYWORD_BOOLEAN:
        return std::make_unique<ast::type::primitive_type_ast_node>(codesh::definition::primitive_type::BOOLEAN);

    case codesh::token_group::IDENTIFIER:
    {
        const std::string custom_name = dynamic_cast<codesh::identifier_token *>(type_token.get())->get_content();
        auto custom_type_node = std::make_unique<ast::type::custom_type_ast_node>();
        custom_type_node->set_name(custom_name);
        return custom_type_node;
    }

    default: // maybe its not needed
        throw std::runtime_error("Unexpected token: expected type");
    }
}
