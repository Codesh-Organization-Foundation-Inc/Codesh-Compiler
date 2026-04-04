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


static void parse_class_scope(std::queue<std::unique_ptr<codesh::token>> &tokens,
        ast::type_decl::class_declaration_ast_node *class_node);

[[nodiscard]] static std::pair<
    std::unique_ptr<ast::type_decl::field_declaration_ast_node>,
    std::unique_ptr<ast::op::assignment::assignment_operator_ast_node>
> parse_field_declaration(std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::lexer::code_position declaration_pos);

static void parse_method_signature_to(
        ast::type_decl::type_declaration_ast_node &type_decl, codesh::lexer::code_position code_position,
        std::queue<std::unique_ptr<codesh::token>> &tokens);
static std::unique_ptr<ast::method::method_declaration_ast_node> parse_method_signature(
        codesh::lexer::code_position code_position, std::queue<std::unique_ptr<codesh::token>> &tokens);
static std::unique_ptr<ast::method::constructor_declaration_ast_node> parse_constructor_signature(
        codesh::lexer::code_position code_position, ast::type_decl::type_declaration_ast_node &type_decl,
        std::queue<std::unique_ptr<codesh::token>> &tokens);
static void parse_method_signature_continuation(ast::method::method_declaration_ast_node &method_decl,
        codesh::lexer::code_position code_position, std::queue<std::unique_ptr<codesh::token>> &tokens);
static void consume_throws(std::queue<std::unique_ptr<codesh::token>> &tokens,
                           ast::method::method_declaration_ast_node &method_decl);


std::unique_ptr<ast::type_decl::class_declaration_ast_node> codesh::parser::parse_class_declaration(
    lexer::code_position code_position, std::queue<std::unique_ptr<token>> &tokens)
{
    if (!util::consuming_check(tokens, token_group::KEYWORD_NAME))
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(
            blasphemy::details::NO_KEYWORD_NAME,
            blasphemy::blasphemy_type::SYNTAX,
            code_position
        );
    }

    // Get name
    const std::unique_ptr<identifier_token> name_token = util::consume_identifier_token(tokens);
    if (!name_token)
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(
            blasphemy::details::NO_IDENTIFIER,
            blasphemy::blasphemy_type::SYNTAX,
            code_position
        );
    }

    auto node = std::make_unique<ast::type_decl::class_declaration_ast_node>(
        code_position,
        definition::fully_qualified_name(name_token->get_code_position(), name_token->get_content())
    );


    if (util::consuming_check(tokens, token_group::KEYWORD_EXTENDS))
    {
        const std::unique_ptr<identifier_token> super_name = util::consume_identifier_token(tokens);

        auto super_type = std::make_unique<ast::type::custom_type_ast_node>(
            super_name->get_code_position(),
            definition::fully_qualified_name(super_name->get_code_position(), super_name->get_content())
        );

        node->set_super_class(std::move(super_type));
    }

    if (util::consuming_check(tokens, token_group::KEYWORD_IMPLEMENTS))
    {
        do
        {
            const auto interface_name = util::consume_identifier_token(tokens);

            auto interface_type = std::make_unique<ast::type::custom_type_ast_node>(
                interface_name->get_code_position(),
                definition::fully_qualified_name(
                    interface_name->get_code_position(),
                    interface_name->get_content()
                )
            );

            node->add_interface(std::move(interface_type));

        }
        while (util::consuming_check(tokens, token_group::PUNCTUATION_ARG_SEPARATOR));
    }


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
                parse_method_signature_to(*class_node, let_pos, tokens);
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
                auto [var_decl, var_assignment] =
                    parse_field_declaration(tokens, let_pos);

                if (var_assignment != nullptr)
                {
                    var_decl->set_value(var_assignment->release_right());
                }

                class_node->add_field(std::move(var_decl));

                break;
            }

            break;
        }

        case codesh::token_group::SCOPE_END: {
            tokens.pop();
            return;
        }

        default: {
            codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
                //TODO: Rename to DOES_NOT
                codesh::blasphemy::details::TOKEN_DOESNT_EXIST,
                codesh::blasphemy::blasphemy_type::SYNTAX, tokens.front()->get_code_position()
            );
            return;
        }

        }
    }

    codesh::blasphemy::get_blasphemy_collector().add_blasphemy(codesh::blasphemy::details::NO_SCOPE_END,
        codesh::blasphemy::blasphemy_type::SYNTAX, class_node->get_code_position());
}

static std::pair<
    std::unique_ptr<ast::type_decl::field_declaration_ast_node>,
    std::unique_ptr<ast::op::assignment::assignment_operator_ast_node>
> parse_field_declaration(std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::lexer::code_position declaration_pos)
{
    auto decl_node = std::make_unique<ast::type_decl::field_declaration_ast_node>(declaration_pos);
    auto assignment = parser::parse_variable_declaration(
        *decl_node,
        declaration_pos,
        tokens,
        parser::var_decl_assignment_policy::ALLOW
    );
    parser::util::ensure_end_op(tokens);

    return {std::move(decl_node), std::move(assignment)};
}

static void parse_method_signature_to(
        ast::type_decl::type_declaration_ast_node &type_decl, const codesh::lexer::code_position code_position,
        std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    bool is_constructor = false;

    // ושמו
    if (!parser::util::consuming_check(tokens, codesh::token_group::KEYWORD_NAME))
    {
        is_constructor = parser::util::consuming_check(tokens, codesh::token_group::KEYWORD_CONSTRUCTOR);

        if (!is_constructor)
        {
            codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
                codesh::blasphemy::details::NO_KEYWORD_NAME_OR_CONSTRUCTOR,
                codesh::blasphemy::blasphemy_type::SYNTAX,
                code_position
            );
        }
    }

    if (is_constructor)
    {
        type_decl.add_constructor(parse_constructor_signature(code_position, type_decl, tokens));
    }
    else
    {
        type_decl.add_method(parse_method_signature(code_position, tokens));
    }
}

static std::unique_ptr<ast::method::method_declaration_ast_node> parse_method_signature(
        const codesh::lexer::code_position code_position, std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    // * (the name)
    const std::unique_ptr<codesh::identifier_token> name_token = parser::util::consume_identifier_token(tokens);

    auto method_decl = std::make_unique<ast::method::method_declaration_ast_node>(
        code_position,
        codesh::definition::fully_qualified_name(name_token->get_code_position(), name_token->get_content())
    );

    parse_method_signature_continuation(*method_decl, code_position, tokens);
    return method_decl;
}

static std::unique_ptr<ast::method::constructor_declaration_ast_node> parse_constructor_signature(
        const codesh::lexer::code_position code_position,
        ast::type_decl::type_declaration_ast_node &type_decl,
        std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    auto custom_type = std::make_unique<ast::type::custom_type_ast_node>(code_position, type_decl);

    auto method_decl = std::make_unique<ast::method::constructor_declaration_ast_node>(
        code_position,
        std::move(custom_type)
    );

    parse_method_signature_continuation(*method_decl, code_position, tokens);
    return method_decl;
}

static void parse_method_signature_continuation(ast::method::method_declaration_ast_node &method_decl,
        const codesh::lexer::code_position code_position, std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    // Attributes
    method_decl.set_attributes(parser::parse_modifiers(code_position, tokens));

    // Parameters
    auto params = parser::parse_parameter_list(tokens);
    for (auto &param : params)
    {
        method_decl.add_parameter(std::move(param));
    }


    bool did_capture_scope_begin = false;

    // If there is a return type, use it.
    if (parser::util::consuming_check(tokens, codesh::token_group::KEYWORD_RETURN))
    {
        method_decl.set_return_type(parser::util::parse_type(tokens));
    }
    // If there isn't, default to using void.
    // Skip this process for constructors as they already define it
    else if (!dynamic_cast<const ast::method::constructor_declaration_ast_node *>(&method_decl))
    {
        // Blame the void return type on the next scope begin:
        std::unique_ptr<codesh::token> scope_begin_token;
        did_capture_scope_begin = parser::util::consuming_check(tokens, codesh::token_group::SCOPE_BEGIN,
            scope_begin_token);

        method_decl.set_return_type(
            std::make_unique<ast::type::primitive_type_ast_node>(
                scope_begin_token ? scope_begin_token->get_code_position() : code_position,
                codesh::definition::primitive_type::VOID
            )
        );
    }

    if (std::unique_ptr<codesh::token> throws_token;
        parser::util::consuming_check(tokens, codesh::token_group::KEYWORD_THROWS, throws_token))
    {
        consume_throws(tokens, method_decl);
    }


    if (!did_capture_scope_begin && !parser::util::consuming_check(tokens, codesh::token_group::SCOPE_BEGIN))
    {
        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
            codesh::blasphemy::details::NO_SCOPE_BEGIN,
            codesh::blasphemy::blasphemy_type::SYNTAX,
            code_position
        );
    }

    parser::parse_method_scope(tokens, method_decl.get_method_scope());
}

static void consume_throws(std::queue<std::unique_ptr<codesh::token>> &tokens,
                           ast::method::method_declaration_ast_node &method_decl)
{
    do
    {
        const auto sin_name = parser::util::consume_identifier_token(tokens);

        auto exception_type = std::make_unique<ast::type::custom_type_ast_node>(
            sin_name->get_code_position(),
            codesh::definition::fully_qualified_name(
                sin_name->get_code_position(),
                sin_name->get_content()
            )
        );

        method_decl.get_sins_thrown().push_back(std::move(exception_type));
    }
    while (parser::util::consuming_check(tokens, codesh::token_group::PUNCTUATION_ARG_SEPARATOR));
}

std::vector<std::unique_ptr<ast::local_variable_declaration_ast_node>> codesh::parser::parse_parameter_list(
        std::queue<std::unique_ptr<token>> &tokens)
{
    std::vector<std::unique_ptr<ast::local_variable_declaration_ast_node>> results;

    std::unique_ptr<token> arg_declaration_token;
    while (util::consuming_check(tokens, token_group::KEYWORD_TAKES, arg_declaration_token))
    {
        // Parse parameter type
        std::unique_ptr<ast::type::type_ast_node> param_type = util::parse_type(tokens);

        // ושמו
        if (!util::consuming_check(tokens, token_group::KEYWORD_NAME))
        {
            blasphemy::get_blasphemy_collector().add_blasphemy(
                blasphemy::details::NO_KEYWORD_NAME,
                blasphemy::blasphemy_type::SYNTAX,
                arg_declaration_token->get_code_position()
            );
        }

        // * (the name)
        const std::unique_ptr<identifier_token> token_name = util::consume_identifier_token(tokens);

        auto param = std::make_unique<ast::local_variable_declaration_ast_node>(
            arg_declaration_token->get_code_position()
        );
        param->set_type(std::move(param_type));
        param->set_name(token_name->get_content());

        results.emplace_back(std::move(param));
    }

    return results;
}
