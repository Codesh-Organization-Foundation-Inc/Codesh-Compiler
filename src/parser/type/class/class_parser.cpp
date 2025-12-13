#include "class_parser.h"

#include "../../ast/local_variable_declaration_ast_node.h"
#include "../../ast/method/method_declaration_ast_node.h"
#include "../../ast/type/primitive_type_ast_node.h"
#include "../../ast/type_declaration/class_declaration_ast_node.h"
#include "../../util.h"
#include "../type_parser.h"
#include "../../../blasphemies/blasphemy_collector.h"
#include "../../../blasphemies/blasphemy_details.h"

#include "method_parser.h"

namespace ast = codesh::ast;
namespace parser = codesh::parser;

static void parse_field_scope(std::queue<std::unique_ptr<codesh::token>> &tokens);

static void parse_class_scope(std::queue<std::unique_ptr<codesh::token>> &tokens,
        ast::type_decl::class_declaration_ast_node *class_node);

static std::unique_ptr<ast::method::method_declaration_ast_node> parse_method_signature(
        std::queue<std::unique_ptr<codesh::token>> &tokens);


std::unique_ptr<ast::type_decl::class_declaration_ast_node> codesh::parser::parse_class_declaration(
        std::queue<std::unique_ptr<token>> &tokens)
{
    if (!util::consuming_check(tokens, token_group::KEYWORD_NAME))
    {
        error::get_blasphemy_collector().add_blasphemy(error::blasphemy_details::NO_KEYWORD_NAME,
            error::blasphemy_type::SYNTAX);
    }

    // Get name
    const std::unique_ptr<identifier_token> name_token = util::consume_identifier_token(tokens,
        error::blasphemy_details::NO_IDENTIFIER);
    if (!name_token)
    {
        error::get_blasphemy_collector().add_blasphemy(error::blasphemy_details::NO_IDENTIFIER,
            error::blasphemy_type::SYNTAX);
    }

    auto node = std::make_unique<ast::type_decl::class_declaration_ast_node>(name_token->get_content());


    // Get attributes
    node->set_attributes(parse_modifiers(tokens));


    // Start scope
    if (util::consuming_check(tokens, token_group::SCOPE_BEGIN))
    {
        error::get_blasphemy_collector().add_blasphemy(error::blasphemy_details::NO_SCOPE_BEGIN,
            error::blasphemy_type::SYNTAX);
    }

    parse_class_scope(tokens, node.get());


    return node;
}


static void parse_class_scope(std::queue<std::unique_ptr<codesh::token>> &tokens,
        ast::type_decl::class_declaration_ast_node *const class_node)
{
    while (!tokens.empty())
    {
        switch (tokens.front()->get_group())
        {
        case codesh::token_group::KEYWORD_LET: {
            tokens.pop();

            switch (tokens.front()->get_group())
            {
            case codesh::token_group::KEYWORD_METHOD:
            {
                class_node->add_method(
                    parse_method_signature(tokens)
                );

                break;
            }
            case codesh::token_group::KEYWORD_CLASS:
            {
                // TODO: Support ויהי עצם
                throw std::runtime_error("Inner classes not yet supported");
            }

            default:
                // Assume fields by default as they can either be custom types (identifiers)
                // or primitives (other tokens).
                parse_field_scope(tokens);
                break;
            }
            break;
        }

        case codesh::token_group::SCOPE_END: {
            tokens.pop();
            return;
        }

        default: {
            codesh::error::get_blasphemy_collector().add_blasphemy(codesh::error::blasphemy_details::TOKEN_NOT_EXIST,
                                                                   codesh::error::blasphemy_type::SYNTAX);
            return;
        }

        }
    }

    codesh::error::get_blasphemy_collector().add_blasphemy(codesh::error::blasphemy_details::NO_SCOPE_END,
        codesh::error::blasphemy_type::SYNTAX);
}

static void parse_field_scope(std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    const codesh::token_group type_token = parser::util::consume_token(tokens,
            codesh::error::blasphemy_details::NO_TYPE)->get_group();


    if (type_token != codesh::token_group::IDENTIFIER)
    {
        // TODO: Parse token group as type and see if it is a primitive
        constexpr bool isPrimitive = false;

        if (!isPrimitive && type_token != codesh::token_group::KEYWORD_VAR)
        {
            codesh::error::get_blasphemy_collector().add_blasphemy(codesh::error::blasphemy_details::NO_IDENTIFIER,
                codesh::error::blasphemy_type::SYNTAX);
        }
    }

    //TODO: Support fields
    throw std::runtime_error("Fields not yet supported");
}

static std::unique_ptr<ast::method::method_declaration_ast_node> parse_method_signature(
        std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    tokens.pop();

    // ושמו
    if (!parser::util::consuming_check(tokens, codesh::token_group::KEYWORD_NAME))
    {
        codesh::error::get_blasphemy_collector().add_blasphemy(codesh::error::blasphemy_details::NO_KEYWORD_NAME,
            codesh::error::blasphemy_type::SYNTAX);
    }

    // * (the name)
    const std::unique_ptr<codesh::identifier_token> name_token = parser::util::consume_identifier_token(tokens,
        codesh::error::blasphemy_details::NO_IDENTIFIER);

    auto method_node = std::make_unique<ast::method::method_declaration_ast_node>();

    method_node->set_name(name_token->get_content());

    // Get attributes
    method_node->set_attributes(parser::parse_modifiers(tokens));


    while (parser::util::consuming_check(tokens, codesh::token_group::KEYWORD_TAKES))
    {
        // Parse parameter type
        std::unique_ptr<ast::type::type_ast_node> param_type = parser::util::parse_type(tokens);

        // ושמו
        if (parser::util::consuming_check(tokens, codesh::token_group::KEYWORD_NAME))
        {
            codesh::error::get_blasphemy_collector().add_blasphemy(
                codesh::error::blasphemy_details::NO_KEYWORD_NAME,
                codesh::error::blasphemy_type::SYNTAX
            );
        }

        // * (the name)
        const std::unique_ptr<codesh::identifier_token> token_name = parser::util::consume_identifier_token(tokens,
            codesh::error::blasphemy_details::NO_IDENTIFIER);

        auto param = std::make_unique<ast::local_variable_declaration_ast_node>();
        param->set_type(std::move(param_type));
        param->set_name(token_name->get_content());

        method_node->get_parameters().push_back(std::move(param));
    }


    // If no וישב, return type = void:
    if (parser::util::consuming_check(tokens, codesh::token_group::KEYWORD_RETURN))
    {
        method_node->set_return_type(parser::util::parse_type(tokens));
    }
    else
    {
        method_node->set_return_type(
            std::make_unique<ast::type::primitive_type_ast_node>(
                codesh::definition::primitive_type::VOID
            )
        );
    }


    if (!parser::util::consuming_check(tokens, codesh::token_group::SCOPE_BEGIN))
    {
        codesh::error::get_blasphemy_collector().add_blasphemy(
            codesh::error::blasphemy_details::NO_SCOPE_BEGIN,
            codesh::error::blasphemy_type::SYNTAX
        );
    }

    parser::parse_method(tokens);

    return std::move(method_node);
}
