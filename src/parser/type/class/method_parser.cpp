#include "method_parser.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "parser/ast/method/method_declaration_ast_node.h"
#include "parser/ast/method/operation/block/if_ast_node.h"
#include "parser/ast/method/operation/block/while_ast_node.h"
#include "parser/ast/method/operation/method_call_ast_node.h"
#include "parser/ast/operator/assignment/addition_assignment_operator_ast_node.h"
#include "parser/ast/operator/assignment/assign_operator_ast_node.h"
#include "parser/util.h"
#include "parser/value_parser.h"
#include "parser/type/type_parser.h"
#include "fmt/format.h"

static void parse_methods_call_parameters(std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::ast::method::operation::method_call_ast_node &method_call);

static std::optional<codesh::blasphemy::code_position> check_consume_scope_begin(
        std::queue<std::unique_ptr<codesh::token>> &tokens);

static codesh::ast::block::conditioned_scope_container parse_conditioned_scope(
        std::queue<std::unique_ptr<codesh::token>> &tokens, codesh::ast::method::method_scope_ast_node &method_scope,
        codesh::blasphemy::code_position fallback_position);

void codesh::parser::parse_method_scope(std::queue<std::unique_ptr<token>> &tokens,
        ast::method::method_scope_ast_node &method_scope)
{
    while (!tokens.empty())
    {
        //TODO: Implement
        switch (tokens.front()->get_group())
        {
        case token_group::KEYWORD_FUNCTION_CALL:
            method_scope.add_statement(parse_methods_call(tokens));
            break;

        case token_group::KEYWORD_LET: {
            auto result = parse_variable_declaration(tokens, var_decl_assignment_policy::REQUIRE);
            method_scope.add_local_variable(std::move(result.first));

            // Add the produced assignment statement to the method body if one was generated
            if (result.second != nullptr)
            {
                method_scope.add_statement(std::move(result.second));
            }

            break;
        }

        case token_group::OPERATOR_ADDITION_ASSIGNMENT:
        case token_group::OPERATOR_DIVISION_ASSIGNMENT:
        case token_group::OPERATOR_MODULO_ASSIGNMENT:
        case token_group::OPERATOR_MULTIPLICATION_ASSIGNMENT:
        case token_group::OPERATOR_SUBTRACTION_ASSIGNMENT:

        case token_group::OPERATOR_ADDITION:
        case token_group::OPERATOR_SUBTRACTION:
        case token_group::OPERATOR_MULTIPLICATION:
        case token_group::OPERATOR_DIVISION:
        case token_group::OPERATOR_MODULO:
            method_scope.add_statement(parse_value(tokens));
            if (!util::consuming_check(tokens, token_group::PUNCTUATION_END_OP))
            {
                blasphemy::get_blasphemy_collector().add_blasphemy(blasphemy::details::NO_PUNCTUATION_END_OP,
                    blasphemy::blasphemy_type::SYNTAX, tokens.empty() ? blasphemy::NO_CODE_POS : tokens.front()->get_code_position());
            }
            break;

        case token_group::KEYWORD_IF:
            method_scope.add_statement(parse_if_statement(tokens, method_scope));
            break;

        case token_group::KEYWORD_WHILE:
            method_scope.add_statement(parse_while_statement(tokens, method_scope));
            break;

        case token_group::KEYWORD_FOR:
            method_scope.add_statement(parse_for_statement(tokens, method_scope));
            break;

        case token_group::SCOPE_END:
            tokens.pop();
            method_scope.mark_end();
            return;

        default: {
            const auto token_name = util::get_token_display_name(*tokens.front());

            blasphemy::get_blasphemy_collector().add_blasphemy(
                fmt::format(
                    "{}: {}",
                    blasphemy::details::UNEXPECTED_TOKEN,
                    token_name
                ),
                blasphemy::blasphemy_type::SYNTAX,
                tokens.front()->get_code_position()
            );

            tokens.pop();
        }
        }
    }

    blasphemy::get_blasphemy_collector().add_blasphemy(blasphemy::details::NO_SCOPE_END,
        blasphemy::blasphemy_type::SYNTAX, method_scope.get_code_position());
}

std::unique_ptr<codesh::ast::method::operation::method_call_ast_node> codesh::parser::parse_methods_call(
        std::queue<std::unique_ptr<token>> &tokens)
{
    auto call_pos = tokens.front()->get_code_position();
    tokens.pop();

    auto method_call_node = std::make_unique<ast::method::operation::method_call_ast_node>(call_pos);

    util::parse_fqcn(tokens, method_call_node->get_fqcn());

    if (util::consuming_check(tokens, token_group::OPEN_PARENTHESIS))
    {
        parse_methods_call_parameters(tokens, *method_call_node);
    }

    util::ensure_end_op(tokens);
    return method_call_node;
}

std::unique_ptr<codesh::ast::block::if_ast_node> codesh::parser::parse_if_statement(
        std::queue<std::unique_ptr<token>> &tokens,
        ast::method::method_scope_ast_node &method_scope)
{
    auto if_pos = tokens.front()->get_code_position();
    tokens.pop();

    auto if_node = std::make_unique<ast::block::if_ast_node>(
        if_pos,
        parse_conditioned_scope(tokens, method_scope, if_pos)
    );

    std::unique_ptr<token> else_token;
    while (util::consuming_check(tokens, token_group::KEYWORD_ELSE_IF, else_token))
    {
        if_node->add_else_if_branch(parse_conditioned_scope(
            tokens,
            method_scope,
            else_token->get_code_position()
        ));
    }

    if (util::consuming_check(tokens, token_group::KEYWORD_ELSE, else_token))
    {
        const auto scope_pos = check_consume_scope_begin(tokens);
        auto &else_scope = method_scope.create_method_scope(
            scope_pos.value_or(else_token->get_code_position())
        );

        parse_method_scope(tokens, else_scope);

        if_node->set_else_branch(else_scope);
    }

    return if_node;
}

static codesh::ast::block::conditioned_scope_container parse_conditioned_scope(
        std::queue<std::unique_ptr<codesh::token>> &tokens, codesh::ast::method::method_scope_ast_node &method_scope,
        codesh::blasphemy::code_position fallback_position)
{
    auto condition = codesh::parser::parse_value(tokens);

    const auto scope_pos = check_consume_scope_begin(tokens);
    auto &scope = method_scope.create_method_scope(
        scope_pos.value_or(fallback_position)
    );

    codesh::parser::parse_method_scope(tokens, scope);

    return {std::move(condition), scope};
}

std::unique_ptr<codesh::ast::block::while_ast_node> codesh::parser::parse_while_statement(
        std::queue<std::unique_ptr<token>> &tokens, ast::method::method_scope_ast_node &method_scope)
{
    auto while_pos = tokens.front()->get_code_position();
    tokens.pop();

    auto condition = parse_value(tokens);

    const auto scope_pos = check_consume_scope_begin(tokens);

    auto &while_scope = method_scope.create_method_scope(scope_pos.value_or(while_pos));
    parse_method_scope(tokens, while_scope);

    return std::make_unique<ast::block::while_ast_node>(
        while_pos,
        std::move(condition),
        while_scope
    );
}

std::unique_ptr<codesh::ast::block::for_ast_node> codesh::parser::parse_for_statement(
    std::queue<std::unique_ptr<token>> &tokens,
    ast::method::method_scope_ast_node &method_scope)
{
    auto for_pos = tokens.front()->get_code_position();
    auto iterator_decl =
        parse_variable_declaration(tokens, var_decl_assignment_policy::FORBID).first;

    if (!util::consuming_check(tokens, token_group::KEYWORD_FROM))
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(
            blasphemy::details::NO_KEYWORD_FROM,
            blasphemy::blasphemy_type::SYNTAX,
            for_pos
        );
    }

    auto collection = parse_value(tokens);


    const auto scope_pos = check_consume_scope_begin(tokens);

    auto &for_scope = method_scope.create_method_scope(scope_pos.value_or(for_pos));
    for_scope.add_local_variable(std::move(iterator_decl));
    parse_method_scope(tokens, for_scope);

    return std::make_unique<ast::block::for_ast_node>(
        for_pos,
        std::move(iterator_decl),
        std::move(collection),
        for_scope
    );
}


static std::optional<codesh::blasphemy::code_position> check_consume_scope_begin(
        std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    std::unique_ptr<codesh::token> token;
    if (!codesh::parser::util::consuming_check(tokens, codesh::token_group::SCOPE_BEGIN, token))
    {
        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
            codesh::blasphemy::details::NO_SCOPE_BEGIN,
            codesh::blasphemy::blasphemy_type::SYNTAX,
            tokens.empty() ? codesh::blasphemy::NO_CODE_POS : tokens.front()->get_code_position()
        );

        return std::nullopt;
    }

    return token->get_code_position();
}

std::pair<
    std::unique_ptr<codesh::ast::local_variable_declaration_ast_node>,
    std::unique_ptr<codesh::ast::op::assignment::assignment_operator_ast_node>
> codesh::parser::parse_variable_declaration(std::queue<std::unique_ptr<token>> &tokens,
        const var_decl_assignment_policy assignment_policy)
{
    const auto declaration_pos = tokens.front()->get_code_position();
    tokens.pop();

    auto variable_decl_ast_node_type = util::parse_type(tokens);
    auto variable_decl_ast_node = std::make_unique<ast::local_variable_declaration_ast_node>(declaration_pos);
    variable_decl_ast_node->set_type(std::move(variable_decl_ast_node_type));

    if (!util::consuming_check(tokens, token_group::KEYWORD_NAME))
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(blasphemy::details::NO_KEYWORD_NAME,
            blasphemy::blasphemy_type::SYNTAX, declaration_pos);
    }

    const auto name_token = util::consume_identifier_token(tokens);

    variable_decl_ast_node->set_name(name_token->get_content());
    variable_decl_ast_node->set_attributes(parse_modifiers(declaration_pos, tokens));


    // Value assignment
    std::unique_ptr<token> assignment_token;
    const bool has_val_assignment = util::consuming_check(tokens, token_group::KEYWORD_LET, assignment_token);

    if (assignment_policy == var_decl_assignment_policy::REQUIRE && !has_val_assignment)
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(blasphemy::details::NO_KEYWORD_LET,
                                                           blasphemy::blasphemy_type::SYNTAX, declaration_pos);
    }

    if (assignment_policy == var_decl_assignment_policy::FORBID)
    {
        if (has_val_assignment)
        {
            blasphemy::get_blasphemy_collector().add_blasphemy(
                fmt::format(
                "{}: {}",
                    blasphemy::details::UNEXPECTED_TOKEN,
                    util::get_token_display_name(*assignment_token)
                ),
                blasphemy::blasphemy_type::SYNTAX,
                assignment_token->get_code_position()
            );
        }

        return {std::move(variable_decl_ast_node), nullptr};
    }


    std::unique_ptr<ast::op::assignment::assign_operator_ast_node> val_assignment = nullptr;

    if (has_val_assignment)
    {
        val_assignment = std::make_unique<ast::op::assignment::assign_operator_ast_node>(
            assignment_token->get_code_position(),

            std::make_unique<variable_reference_ast_node>(
                declaration_pos,
                *variable_decl_ast_node
            ),
            parse_value(tokens)
        );

        util::ensure_end_op(tokens);
    }

    return {std::move(variable_decl_ast_node), std::move(val_assignment)};
}

static void parse_methods_call_parameters(std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::ast::method::operation::method_call_ast_node &method_call)
{
    while (!codesh::parser::util::consuming_check(tokens, codesh::token_group::CLOSE_PARENTHESIS))
    {
        method_call.get_arguments().push_back(codesh::parser::parse_value(tokens));

        if (codesh::parser::util::consuming_check(tokens, codesh::token_group::PUNCTUATION_ARG_SEPARATOR))
            continue;

        // If there are no more arguments, there shouldn't be anything else besides a closing parenthesis.
        if (!codesh::parser::util::peeking_check(tokens, codesh::token_group::CLOSE_PARENTHESIS))
        {
            codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
                tokens.empty() ? codesh::blasphemy::details::UNEXPECTED_TOKEN : fmt::format(
                    "{}: {}",
                    codesh::blasphemy::details::UNEXPECTED_TOKEN,
                    codesh::parser::util::get_token_display_name(*tokens.front())
                ),
                codesh::blasphemy::blasphemy_type::SYNTAX,
                tokens.empty() ? codesh::blasphemy::NO_CODE_POS : tokens.front()->get_code_position()
            );
        }
    }
}
