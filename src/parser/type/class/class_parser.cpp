#include "class_parser.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "parser/ast/local_variable_declaration_ast_node.h"
#include "parser/ast/method/method_declaration_ast_node.h"
#include "parser/ast/type/primitive_type_ast_node.h"
#include "parser/ast/type_declaration/class_declaration_ast_node.h"
#include "parser/util.h"
#include "parser/type/type_parser.h"

#include "method_parser.h"

namespace ast = codesh::ast;
namespace parser = codesh::parser;

static void parse_field_scope(std::queue<std::unique_ptr<codesh::token>> &tokens);

static void parse_class_scope(std::queue<std::unique_ptr<codesh::token>> &tokens,
        ast::type_decl::class_declaration_ast_node *class_node);

static std::unique_ptr<ast::method::method_declaration_ast_node> parse_method_signature(
        std::queue<std::unique_ptr<codesh::token>> &tokens);


std::unique_ptr<ast::type_decl::class_declaration_ast_node> codesh::parser::parse_class_declaration(
        std::queue<std::unique_ptr<token>> &tokens, const definition::basad_type basad_type)
{
    if (!util::consuming_check(tokens, token_group::KEYWORD_NAME))
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(blasphemy::details::NO_KEYWORD_NAME,
            blasphemy::blasphemy_type::SYNTAX);
    }

    // Get name
    const std::unique_ptr<identifier_token> name_token = util::consume_identifier_token(tokens);
    if (!name_token)
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(blasphemy::details::NO_IDENTIFIER,
            blasphemy::blasphemy_type::SYNTAX);
    }

    auto node = std::make_unique<ast::type_decl::class_declaration_ast_node>(
        definition::fully_qualified_name(name_token->get_content()), basad_type
    );


    // Get attributes
    node->set_attributes(parse_modifiers(tokens));


    // Start scope
    if (!util::consuming_check(tokens, token_group::SCOPE_BEGIN))
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(blasphemy::details::NO_SCOPE_BEGIN,
            blasphemy::blasphemy_type::SYNTAX);
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
            codesh::blasphemy::get_blasphemy_collector().add_blasphemy(codesh::blasphemy::details::TOKEN_DOESNT_EXIST,
                                                                   codesh::blasphemy::blasphemy_type::SYNTAX);
            return;
        }

        }
    }

    codesh::blasphemy::get_blasphemy_collector().add_blasphemy(codesh::blasphemy::details::NO_SCOPE_END,
        codesh::blasphemy::blasphemy_type::SYNTAX);
}

static void parse_field_scope(std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    const codesh::token_group type_token = parser::util::consume_token(tokens,
            codesh::blasphemy::details::NO_TYPE)->get_group();


    if (type_token != codesh::token_group::IDENTIFIER)
    {
        // TODO: Parse token group as type and see if it is a primitive
        constexpr bool isPrimitive = false;

        if (!isPrimitive && type_token != codesh::token_group::KEYWORD_VAR)
        {
            codesh::blasphemy::get_blasphemy_collector().add_blasphemy(codesh::blasphemy::details::NO_IDENTIFIER,
                codesh::blasphemy::blasphemy_type::SYNTAX);
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
        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(codesh::blasphemy::details::NO_KEYWORD_NAME,
            codesh::blasphemy::blasphemy_type::SYNTAX);
    }

    // * (the name)
    const std::unique_ptr<codesh::identifier_token> name_token = parser::util::consume_identifier_token(tokens);

    auto method_node = std::make_unique<ast::method::method_declaration_ast_node>(
        codesh::definition::fully_qualified_name(name_token->get_content())
    );

    // Get attributes
    method_node->set_attributes(parser::parse_modifiers(tokens));


    while (parser::util::consuming_check(tokens, codesh::token_group::KEYWORD_TAKES))
    {
        // Parse parameter type
        std::unique_ptr<ast::type::type_ast_node> param_type = parser::util::parse_type(tokens);

        // ושמו
        if (!parser::util::consuming_check(tokens, codesh::token_group::KEYWORD_NAME))
        {
            codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
                codesh::blasphemy::details::NO_KEYWORD_NAME,
                codesh::blasphemy::blasphemy_type::SYNTAX
            );
        }

        // * (the name)
        const std::unique_ptr<codesh::identifier_token> token_name = parser::util::consume_identifier_token(tokens);

        auto param = std::make_unique<ast::local_variable_declaration_ast_node>();
        param->set_type(std::move(param_type));
        param->set_name(token_name->get_content());

        method_node->add_parameter(std::move(param));
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
        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
            codesh::blasphemy::details::NO_SCOPE_BEGIN,
            codesh::blasphemy::blasphemy_type::SYNTAX
        );
    }

    parser::parse_method_scope(tokens, method_node->get_method_scope());

    return method_node;
}
