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
#include "parser/ast/compilation_unit_ast_node.h"
#include "parser/ast/method/constructor_declaration_ast_node.h"

namespace ast = codesh::ast;
namespace parser = codesh::parser;

static void parse_field_scope(std::queue<std::unique_ptr<codesh::token>> &tokens);

static void parse_class_scope(std::queue<std::unique_ptr<codesh::token>> &tokens,
        ast::type_decl::class_declaration_ast_node *class_node);

static std::unique_ptr<ast::method::method_declaration_ast_node> parse_method_signature(
        codesh::blasphemy::code_position code_position, std::queue<std::unique_ptr<codesh::token>> &tokens);

static std::unique_ptr<ast::method::constructor_declaration_ast_node> parse_constructor_signature(
        codesh::blasphemy::code_position code_position, std::queue<std::unique_ptr<codesh::token>> &tokens);


std::unique_ptr<ast::type_decl::class_declaration_ast_node> codesh::parser::parse_class_declaration(
    blasphemy::code_position code_position, std::queue<std::unique_ptr<token>> &tokens)
{
    if (!util::consuming_check(tokens, token_group::KEYWORD_NAME))
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(blasphemy::details::NO_KEYWORD_NAME,
            blasphemy::blasphemy_type::SYNTAX, code_position);
    }

    // Get name
    const std::unique_ptr<identifier_token> name_token = util::consume_identifier_token(tokens);
    if (!name_token)
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(blasphemy::details::NO_IDENTIFIER,
            blasphemy::blasphemy_type::SYNTAX, code_position);
    }

    auto node = std::make_unique<ast::type_decl::class_declaration_ast_node>(
        code_position,
        definition::fully_qualified_name(name_token->get_content())
    );


    // Get attributes
    node->set_attributes(parse_modifiers(code_position, tokens));


    // Start scope
    if (!util::consuming_check(tokens, token_group::SCOPE_BEGIN))
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(blasphemy::details::NO_SCOPE_BEGIN,
            blasphemy::blasphemy_type::SYNTAX, code_position);
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
            const auto let_pos = tokens.front()->get_code_position();
            tokens.pop();

            switch (tokens.front()->get_group())
            {
            case codesh::token_group::KEYWORD_METHOD:
            {
                tokens.pop();
                if (tokens.size() > 1 && parser::util::consuming_check(tokens, codesh::token_group::KEYWORD_CONSTRUCTOR))
                {
                    class_node->add_method(
                        parse_constructor_signature(let_pos, tokens)
                    );
                }
                else
                {
                    class_node->add_method(
                        parse_method_signature(let_pos, tokens)
                    );
                }
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
                                                                   codesh::blasphemy::blasphemy_type::SYNTAX, tokens.front()->get_code_position());
            return;
        }

        }
    }

    codesh::blasphemy::get_blasphemy_collector().add_blasphemy(codesh::blasphemy::details::NO_SCOPE_END,
        codesh::blasphemy::blasphemy_type::SYNTAX, class_node->get_code_position());
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
                codesh::blasphemy::blasphemy_type::SYNTAX, tokens.front()->get_code_position());
        }
    }

    //TODO: Support fields
    throw std::runtime_error("Fields not yet supported");
}

static std::unique_ptr<ast::method::method_declaration_ast_node> parse_method_signature(
        codesh::blasphemy::code_position code_position, std::queue<std::unique_ptr<codesh::token>> &tokens)
{

    // ושמו
    if (!parser::util::consuming_check(tokens, codesh::token_group::KEYWORD_NAME))
    {
        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(codesh::blasphemy::details::NO_KEYWORD_NAME,
            codesh::blasphemy::blasphemy_type::SYNTAX, code_position);
    }

    // * (the name)
    const std::unique_ptr<codesh::identifier_token> name_token = parser::util::consume_identifier_token(tokens);

    auto method_node = std::make_unique<ast::method::method_declaration_ast_node>(
        code_position,
        codesh::definition::fully_qualified_name(name_token->get_content())
    );

    // Get attributes
    method_node->set_attributes(parser::parse_modifiers(code_position, tokens));


    std::unique_ptr<codesh::token> arg_declaration_token;
    while (parser::util::consuming_check(tokens, codesh::token_group::KEYWORD_TAKES, arg_declaration_token))
    {
        // Parse parameter type
        std::unique_ptr<ast::type::type_ast_node> param_type = parser::util::parse_type(tokens);

        // ושמו
        if (!parser::util::consuming_check(tokens, codesh::token_group::KEYWORD_NAME))
        {
            codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
                codesh::blasphemy::details::NO_KEYWORD_NAME,
                codesh::blasphemy::blasphemy_type::SYNTAX,
                arg_declaration_token->get_code_position()
            );
        }

        // * (the name)
        const std::unique_ptr<codesh::identifier_token> token_name = parser::util::consume_identifier_token(tokens);

        auto param = std::make_unique<ast::local_variable_declaration_ast_node>(
            arg_declaration_token->get_code_position()
        );
        param->set_type(std::move(param_type));
        param->set_name(token_name->get_content());

        method_node->add_parameter(std::move(param));
    }


    bool did_capture_scope_begin = false;

    // If no וישב, return type = void:
    if (parser::util::consuming_check(tokens, codesh::token_group::KEYWORD_RETURN))
    {
        method_node->set_return_type(parser::util::parse_type(tokens));
    }
    else
    {
        // Blame the void return type on the next scope begin:
        std::unique_ptr<codesh::token> scope_begin_token;
        did_capture_scope_begin = parser::util::consuming_check(tokens, codesh::token_group::SCOPE_BEGIN,
            scope_begin_token);

        method_node->set_return_type(
            std::make_unique<ast::type::primitive_type_ast_node>(
                scope_begin_token ? scope_begin_token->get_code_position() : code_position,
                codesh::definition::primitive_type::VOID
            )
        );
    }


    if (!did_capture_scope_begin && !parser::util::consuming_check(tokens, codesh::token_group::SCOPE_BEGIN))
    {
        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
            codesh::blasphemy::details::NO_SCOPE_BEGIN,
            codesh::blasphemy::blasphemy_type::SYNTAX,
            code_position
        );
    }

    parser::parse_method_scope(tokens, method_node->get_method_scope());

    return method_node;
}

static std::unique_ptr<ast::method::constructor_declaration_ast_node> parse_constructor_signature(
        codesh::blasphemy::code_position code_position, std::queue<std::unique_ptr<codesh::token>> &tokens)
{

    auto constructor_node = std::make_unique<ast::method::constructor_declaration_ast_node>(
        code_position
    );

    // Get attributes
    constructor_node->set_attributes(parser::parse_modifiers(code_position, tokens));

    std::unique_ptr<codesh::token> arg_declaration_token;

    while (parser::util::consuming_check(tokens, codesh::token_group::KEYWORD_TAKES, arg_declaration_token))
    {
        // Parse parameter type
        std::unique_ptr<ast::type::type_ast_node> param_type = parser::util::parse_type(tokens);

        // ושמו
        if (!parser::util::consuming_check(tokens, codesh::token_group::KEYWORD_NAME))
        {
            codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
                codesh::blasphemy::details::NO_KEYWORD_NAME,
                codesh::blasphemy::blasphemy_type::SYNTAX,
                arg_declaration_token->get_code_position()
            );
        }

        // * (the name)
        const std::unique_ptr<codesh::identifier_token> token_name = parser::util::consume_identifier_token(tokens);

        auto param = std::make_unique<ast::local_variable_declaration_ast_node>(
            arg_declaration_token->get_code_position()
        );
        param->set_type(std::move(param_type));
        param->set_name(token_name->get_content());

        constructor_node->add_parameter(std::move(param));
    }

    if (!parser::util::consuming_check(tokens, codesh::token_group::SCOPE_BEGIN))
    {
        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
            codesh::blasphemy::details::NO_SCOPE_BEGIN,
            codesh::blasphemy::blasphemy_type::SYNTAX,
            code_position
        );
    }

    parser::parse_method_scope(tokens, constructor_node->get_method_scope());

    return constructor_node;
}