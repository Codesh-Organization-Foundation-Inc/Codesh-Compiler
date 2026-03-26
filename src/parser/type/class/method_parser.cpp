#include "method_parser.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "fmt/format.h"
#include "parser/ast/method/method_declaration_ast_node.h"
#include "parser/ast/method/operation/block/if_ast_node.h"
#include "parser/ast/method/operation/block/while_ast_node.h"
#include "parser/ast/method/operation/method_call_ast_node.h"
#include "parser/ast/method/operation/return_ast_node.h"
#include "parser/ast/operator/assignment/addition_assignment_operator_ast_node.h"
#include "parser/type/type_parser.h"
#include "parser/util.h"
#include "parser/value/primitive_value_parser.h"
#include "parser/value/value_parser.h"

static std::optional<codesh::lexer::code_position> check_consume_scope_begin(
        std::queue<std::unique_ptr<codesh::token>> &tokens);

static codesh::ast::block::conditioned_scope_container parse_conditioned_scope(
        std::queue<std::unique_ptr<codesh::token>> &tokens, codesh::ast::method::method_scope_ast_node &method_scope,
        codesh::lexer::code_position fallback_position);

static std::unique_ptr<codesh::ast::block::if_ast_node> parse_if_statement(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::ast::method::method_scope_ast_node &method_scope);

static std::unique_ptr<codesh::ast::block::while_ast_node> parse_while_statement(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::ast::method::method_scope_ast_node &method_scope);

static std::unique_ptr<codesh::ast::block::for_ast_node> parse_for_statement(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::ast::method::method_scope_ast_node &method_scope);

static std::unique_ptr<codesh::ast::method::operation::return_ast_node> parse_return_operator(
        std::queue<std::unique_ptr<codesh::token>> &tokens);



void codesh::parser::parse_method_scope(std::queue<std::unique_ptr<token>> &tokens,
        ast::method::method_scope_ast_node &method_scope)
{
    while (!tokens.empty())
    {
        switch (tokens.front()->get_group())
        {
        case token_group::KEYWORD_FUNCTION_CALL:
            method_scope.add_statement(parse_method_call(tokens));
            util::ensure_end_op(tokens);
            break;

        case token_group::KEYWORD_LET: {
            auto [var_decl, var_assignment] =
                parse_variable_declaration(tokens, var_decl_assignment_policy::REQUIRE);

            method_scope.add_local_variable(std::move(var_decl));

            // Add the produced assignment statement to the method body if one was generated
            if (var_assignment != nullptr)
            {
                method_scope.add_statement(std::move(var_assignment));
            }

            break;
        }

        case token_group::KEYWORD_THIS:
        case token_group::IDENTIFIER:
        case token_group::LITERAL_STRING:
        case token_group::LITERAL_NUMBER_INT:
        case token_group::LITERAL_NUMBER_FLOAT:
        case token_group::LITERAL_NUMBER_DOUBLE:
        case token_group::LITERAL_CHAR:
        case token_group::KEYWORD_TRUE:
        case token_group::KEYWORD_FALSE:
        case token_group::OPERATOR_ADDITION:
        case token_group::OPERATOR_SUBTRACTION:
        case token_group::OPERATOR_MULTIPLICATION:
        case token_group::OPERATOR_DIVISION:
        case token_group::OPERATOR_MODULO:
        case token_group::OPEN_PARENTHESIS:
        case token_group::OPERATOR_MINUS:
        case token_group::OPERATOR_EQUALS:
        case token_group::OPERATOR_NOT_EQUALS:
        case token_group::OPERATOR_GREATER:
        case token_group::OPERATOR_GREATER_EQUALS:
        case token_group::OPERATOR_LESS:
        case token_group::OPERATOR_LESS_EQUALS:
        case token_group::KEYWORD_REPLACE:
        case token_group::OPERATOR_ADDITION_ASSIGNMENT:
        case token_group::OPERATOR_DIVISION_ASSIGNMENT:
        case token_group::OPERATOR_MODULO_ASSIGNMENT:
        case token_group::OPERATOR_MULTIPLICATION_ASSIGNMENT:
        case token_group::OPERATOR_SUBTRACTION_ASSIGNMENT:
        case token_group::OPERATOR_INCREMENT:
        case token_group::OPERATOR_DECREMENT:
        case token_group::KEYWORD_NEW:
            method_scope.add_statement(value::parse_value(tokens));
            if (!util::consuming_check(tokens, token_group::PUNCTUATION_END_OP))
            {
                blasphemy::get_blasphemy_collector().add_blasphemy(blasphemy::details::NO_PUNCTUATION_END_OP,
                    blasphemy::blasphemy_type::SYNTAX, tokens.empty() ? lexer::NO_CODE_POS : tokens.front()->get_code_position());
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

        case token_group::KEYWORD_RETURN:
            method_scope.add_statement(parse_return_operator(tokens));
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

std::unique_ptr<codesh::ast::method::operation::method_call_ast_node> codesh::parser::parse_method_call(
        std::queue<std::unique_ptr<token>> &tokens)
{
    auto call_pos = util::consume_token(tokens)->get_code_position();
    auto method_call_node = std::make_unique<ast::method::operation::method_call_ast_node>(call_pos);

    const auto front_group = tokens.front()->get_group();
    const bool is_literal_receiver =
           front_group == token_group::LITERAL_STRING
        || front_group == token_group::LITERAL_NUMBER_INT
        || front_group == token_group::LITERAL_NUMBER_FLOAT
        || front_group == token_group::LITERAL_NUMBER_DOUBLE
        || front_group == token_group::LITERAL_CHAR
        || front_group == token_group::KEYWORD_TRUE
        || front_group == token_group::KEYWORD_FALSE;

    // ויעש כי־ויעש כי־וכו'
    if (front_group == token_group::OPEN_PARENTHESIS)
    {
        tokens.pop();
        method_call_node->set_receiver(value::parse_value(tokens));
        method_call_node->set_association(ast::var_reference::reference_association::EXPRESSION);
        util::consuming_check(tokens, token_group::CLOSE_PARENTHESIS);

        util::consuming_check(tokens, token_group::PUNCTUATION_DOT);
        util::parse_fqn(tokens, method_call_node->get_fqn());
    }
    else if (is_literal_receiver)
    {
        method_call_node->set_receiver(value::parse_primitive_value(tokens));
        method_call_node->set_association(ast::var_reference::reference_association::EXPRESSION);
        util::consuming_check(tokens, token_group::PUNCTUATION_DOT);
        util::parse_fqn(tokens, method_call_node->get_fqn());
    }
    else
    {
        const auto association = util::parse_association_and_fqn(tokens, method_call_node->get_fqn());
        method_call_node->set_association(association);
    }

    if (util::consuming_check(tokens, token_group::OPEN_PARENTHESIS))
    {
        parse_methods_call_parameters(tokens, *method_call_node);
    }
    return method_call_node;
}

std::unique_ptr<codesh::ast::block::if_ast_node> parse_if_statement(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::ast::method::method_scope_ast_node &method_scope)
{
    auto if_pos = codesh::parser::util::consume_token(tokens)->get_code_position();

    auto if_node = std::make_unique<codesh::ast::block::if_ast_node>(
        if_pos,
        parse_conditioned_scope(tokens, method_scope, if_pos)
    );

    std::unique_ptr<codesh::token> else_token;
    while (codesh::parser::util::consuming_check(tokens, codesh::token_group::KEYWORD_ELSE_IF, else_token))
    {
        if_node->add_else_if_branch(parse_conditioned_scope(
            tokens,
            method_scope,
            else_token->get_code_position()
        ));
    }

    if (codesh::parser::util::consuming_check(tokens, codesh::token_group::KEYWORD_ELSE, else_token))
    {
        const auto scope_pos = check_consume_scope_begin(tokens);
        auto &else_scope = method_scope.create_method_scope(
            scope_pos.value_or(else_token->get_code_position())
        );

        codesh::parser::parse_method_scope(tokens, else_scope);

        if_node->set_else_branch(else_scope);
    }

    return if_node;
}

static codesh::ast::block::conditioned_scope_container parse_conditioned_scope(
        std::queue<std::unique_ptr<codesh::token>> &tokens, codesh::ast::method::method_scope_ast_node &method_scope,
        codesh::lexer::code_position fallback_position)
{
    auto condition = codesh::parser::value::parse_value(tokens);

    const auto scope_pos = check_consume_scope_begin(tokens);
    auto &scope = method_scope.create_method_scope(
        scope_pos.value_or(fallback_position)
    );

    codesh::parser::parse_method_scope(tokens, scope);

    return {std::move(condition), scope};
}

static std::unique_ptr<codesh::ast::block::while_ast_node> parse_while_statement(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::ast::method::method_scope_ast_node &method_scope)
{
    auto while_pos = codesh::parser::util::consume_token(tokens)->get_code_position();

    auto condition = codesh::parser::value::parse_value(tokens);

    const auto scope_pos = check_consume_scope_begin(tokens);

    auto &while_scope = method_scope.create_method_scope(scope_pos.value_or(while_pos));
    codesh::parser::parse_method_scope(tokens, while_scope);

    return std::make_unique<codesh::ast::block::while_ast_node>(
        while_pos,
        std::move(condition),
        while_scope
    );
}

static std::unique_ptr<codesh::ast::method::operation::return_ast_node> parse_return_operator(
        std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    const auto return_pos = codesh::parser::util::consume_token(tokens)->get_code_position();

    auto return_value = codesh::parser::value::parse_value(tokens);

    codesh::parser::util::ensure_end_op(tokens);

    return std::make_unique<codesh::ast::method::operation::return_ast_node>(
        return_pos,
        std::move(return_value)
    );
}

static std::unique_ptr<codesh::ast::block::for_ast_node> parse_for_statement(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::ast::method::method_scope_ast_node &method_scope)
{
    auto for_pos = tokens.front()->get_code_position();
    auto iterator_decl =
        codesh::parser::parse_variable_declaration(tokens, codesh::parser::var_decl_assignment_policy::FORBID).first;

    if (!codesh::parser::util::consuming_check(tokens, codesh::token_group::KEYWORD_FROM))
    {
        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
            codesh::blasphemy::details::NO_KEYWORD_FROM,
            codesh::blasphemy::blasphemy_type::SYNTAX,
            for_pos
        );
    }

    auto collection = codesh::parser::value::parse_value(tokens);

    const auto scope_pos = check_consume_scope_begin(tokens);

    auto &for_scope = method_scope.create_method_scope(scope_pos.value_or(for_pos));
    for_scope.add_local_variable(std::move(iterator_decl));

    // The iterator variable will go into a nested scope so that its scope_begin_marker (IR generation stage)
    // can be placed INSIDE the loop body (after the condition check).
    //
    // This gives them a bytecode_start_pc that correctly EXCLUDES the loop's edge.
    auto &iterator_decl_scope = for_scope.create_method_scope(scope_pos.value_or(for_pos));
    codesh::parser::parse_method_scope(tokens, iterator_decl_scope);

    return std::make_unique<codesh::ast::block::for_ast_node>(
        for_pos,
        std::move(collection),
        for_scope
    );
}


static std::optional<codesh::lexer::code_position> check_consume_scope_begin(
        std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    std::unique_ptr<codesh::token> token;
    if (!codesh::parser::util::consuming_check(tokens, codesh::token_group::SCOPE_BEGIN, token))
    {
        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
            codesh::blasphemy::details::NO_SCOPE_BEGIN,
            codesh::blasphemy::blasphemy_type::SYNTAX,
            tokens.empty() ? codesh::lexer::NO_CODE_POS : tokens.front()->get_code_position()
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
    const auto declaration_pos = util::consume_token(tokens)->get_code_position();

    auto decl_node = std::make_unique<ast::local_variable_declaration_ast_node>(declaration_pos);
    auto assignment = parse_variable_declaration(
        *decl_node,
        declaration_pos,
        tokens,
        assignment_policy
    );

    return {std::move(decl_node), std::move(assignment)};
}

void codesh::parser::parse_methods_call_parameters(std::queue<std::unique_ptr<token>> &tokens,
                                                   ast::method::operation::method_call_ast_node &method_call)
{
    while (!util::consuming_check(tokens, token_group::CLOSE_PARENTHESIS))
    {
        const auto name_token = util::consume_identifier_token(tokens);
        auto value_node = value::parse_value(tokens);

        method_call.get_arguments().emplace_back(name_token->get_content(), std::move(value_node));

        if (util::consuming_check(tokens, token_group::PUNCTUATION_ARG_SEPARATOR))
            continue;

        // If there are no more arguments, there shouldn't be anything else besides a closing parenthesis.
        if (!util::peeking_check(tokens, token_group::CLOSE_PARENTHESIS))
        {
            blasphemy::get_blasphemy_collector().add_blasphemy(
                blasphemy::details::NO_CLOSE_PARENTHESIS,
                blasphemy::blasphemy_type::SYNTAX,
                tokens.empty() ? lexer::NO_CODE_POS : tokens.front()->get_code_position()
            );
            return;
        }
    }
}
