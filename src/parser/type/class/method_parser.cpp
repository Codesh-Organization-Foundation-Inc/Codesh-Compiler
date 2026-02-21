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
#include "parser/ast/operator/assignment/assign_operator_ast_node.h"
#include "parser/type/type_parser.h"
#include "parser/util.h"
#include "parser/value/value_parser.h"

static std::optional<codesh::blasphemy::code_position> check_consume_scope_begin(
        std::queue<std::unique_ptr<codesh::token>> &tokens);

static codesh::ast::block::conditioned_scope_container parse_conditioned_scope(
        std::queue<std::unique_ptr<codesh::token>> &tokens, codesh::ast::method::method_scope_ast_node &method_scope,
        codesh::blasphemy::code_position fallback_position);

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

/**
 * Parses the core of the variable declaration (type, name, modifiers) from the token queue
 */
static std::unique_ptr<codesh::ast::local_variable_declaration_ast_node> parse_variable_decl_core(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::blasphemy::code_position declaration_pos);

/**
 * If an assignment is present and permitted, builds and returns its new assignment node. @c nullptr otherwise.
 */
static std::unique_ptr<codesh::ast::op::assignment::assign_operator_ast_node> handle_var_decl_assignment(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        const codesh::ast::local_variable_declaration_ast_node &decl_node,
        codesh::blasphemy::code_position declaration_pos,
        codesh::parser::var_decl_assignment_policy assignment_policy);

/*
 * Builds an assignment operator node that assigns a parsed RHS value to the given declaration,
 * then consumes the end-of-statement token.
 */
static std::unique_ptr<codesh::ast::op::assignment::assign_operator_ast_node> build_variable_assignment(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        const codesh::ast::local_variable_declaration_ast_node &decl_node,
        codesh::blasphemy::code_position declaration_pos,
        codesh::blasphemy::code_position assignment_pos);


void codesh::parser::parse_method_scope(std::queue<std::unique_ptr<token>> &tokens,
        ast::method::method_scope_ast_node &method_scope)
{
    while (!tokens.empty())
    {
        //TODO: Implement
        switch (tokens.front()->get_group())
        {
        case token_group::KEYWORD_FUNCTION_CALL:
            method_scope.add_statement(parse_method_call(tokens));
            util::ensure_end_op(tokens);
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

        case token_group::KEYWORD_REPLACE:
        case token_group::OPERATOR_ADDITION_ASSIGNMENT:
        case token_group::OPERATOR_DIVISION_ASSIGNMENT:
        case token_group::OPERATOR_MODULO_ASSIGNMENT:
        case token_group::OPERATOR_MULTIPLICATION_ASSIGNMENT:
        case token_group::OPERATOR_SUBTRACTION_ASSIGNMENT:
        case token_group::OPERATOR_INCREMENT:
        case token_group::OPERATOR_DECREMENT:

        case token_group::OPERATOR_ADDITION:
        case token_group::OPERATOR_SUBTRACTION:
        case token_group::OPERATOR_MULTIPLICATION:
        case token_group::OPERATOR_DIVISION:
        case token_group::OPERATOR_MODULO:
            method_scope.add_statement(value::parse_value(tokens));
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

        case token_group::KEYWORD_RETURN:
        {
            method_scope.add_statement(parse_return_operator(tokens));
            break;
        }

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
    auto call_pos = tokens.front()->get_code_position();
    tokens.pop();

    auto method_call_node = std::make_unique<ast::method::operation::method_call_ast_node>(call_pos);

    // Handle explicit receiver: ויעש אנכי ל־methodName(...)
    if (util::consuming_check(tokens, token_group::KEYWORD_THIS))
    {
        util::consuming_check(tokens, token_group::PUNCTUATION_DOT);
        method_call_node->get_fqn().add("this");
    }

    util::parse_fqn(tokens, method_call_node->get_fqn());

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
    auto if_pos = tokens.front()->get_code_position();
    tokens.pop();

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
        codesh::blasphemy::code_position fallback_position)
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
    auto while_pos = tokens.front()->get_code_position();
    tokens.pop();

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
    const auto return_pos = tokens.front()->get_code_position();
    tokens.pop();

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
    codesh::parser::parse_method_scope(tokens, for_scope);

    return std::make_unique<codesh::ast::block::for_ast_node>(
        for_pos,
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

    auto variable_decl_ast_node = parse_variable_decl_core(tokens, declaration_pos);

    return {
        std::move(variable_decl_ast_node),
        handle_var_decl_assignment(tokens, *variable_decl_ast_node, declaration_pos, assignment_policy)
    };
}

static std::unique_ptr<codesh::ast::local_variable_declaration_ast_node> parse_variable_decl_core(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::blasphemy::code_position declaration_pos)
{
    auto decl_node = std::make_unique<codesh::ast::local_variable_declaration_ast_node>(declaration_pos);
    decl_node->set_type(codesh::parser::util::parse_type(tokens));

    if (!codesh::parser::util::consuming_check(tokens, codesh::token_group::KEYWORD_NAME))
    {
        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
            codesh::blasphemy::details::NO_KEYWORD_NAME,
            codesh::blasphemy::blasphemy_type::SYNTAX,
            declaration_pos
        );
    }

    const auto name_token = codesh::parser::util::consume_identifier_token(tokens);
    decl_node->set_name(name_token->get_content());
    decl_node->set_attributes(codesh::parser::parse_modifiers(declaration_pos, tokens));

    return decl_node;
}

static std::unique_ptr<codesh::ast::op::assignment::assign_operator_ast_node> handle_var_decl_assignment(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        const codesh::ast::local_variable_declaration_ast_node &decl_node,
        const codesh::blasphemy::code_position declaration_pos,
        const codesh::parser::var_decl_assignment_policy assignment_policy)
{
    std::unique_ptr<codesh::token> assignment_token;
    const bool has_val_assignment = codesh::parser::util::consuming_check(
        tokens,
        codesh::token_group::KEYWORD_LET,
        assignment_token
    );

    if (assignment_policy == codesh::parser::var_decl_assignment_policy::REQUIRE && !has_val_assignment)
    {
        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
            codesh::blasphemy::details::NO_KEYWORD_LET,
            codesh::blasphemy::blasphemy_type::SYNTAX, declaration_pos
        );
    }

    if (assignment_policy == codesh::parser::var_decl_assignment_policy::FORBID)
    {
        if (has_val_assignment)
        {
            codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
                fmt::format(
                    "{}: {}",
                    codesh::blasphemy::details::UNEXPECTED_TOKEN,
                    codesh::parser::util::get_token_display_name(*assignment_token)
                ),
                codesh::blasphemy::blasphemy_type::SYNTAX,
                assignment_token->get_code_position()
            );
        }

        return nullptr;
    }

    if (has_val_assignment)
    {
        return build_variable_assignment(
            tokens,
            decl_node,
            declaration_pos,
            assignment_token->get_code_position()
        );
    }

    return nullptr;
}

static std::unique_ptr<codesh::ast::op::assignment::assign_operator_ast_node> build_variable_assignment(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        const codesh::ast::local_variable_declaration_ast_node &decl_node,
        codesh::blasphemy::code_position declaration_pos,
        codesh::blasphemy::code_position assignment_pos)
{
    auto assignment = std::make_unique<codesh::ast::op::assignment::assign_operator_ast_node>(
        assignment_pos,
        std::make_unique<variable_reference_ast_node>(declaration_pos, decl_node),
        codesh::parser::value::parse_value(tokens)
    );

    codesh::parser::util::ensure_end_op(tokens);
    return assignment;
}

void codesh::parser::parse_methods_call_parameters(std::queue<std::unique_ptr<token>> &tokens,
                                                   ast::method::operation::method_call_ast_node &method_call)
{
    while (!util::consuming_check(tokens, token_group::CLOSE_PARENTHESIS))
    {
        method_call.get_arguments().push_back(value::parse_value(tokens));

        if (util::consuming_check(tokens, token_group::PUNCTUATION_ARG_SEPARATOR))
            continue;

        // If there are no more arguments, there shouldn't be anything else besides a closing parenthesis.
        if (!util::peeking_check(tokens, token_group::CLOSE_PARENTHESIS))
        {
            blasphemy::get_blasphemy_collector().add_blasphemy(
                tokens.empty() ? blasphemy::details::UNEXPECTED_TOKEN : fmt::format(
                    "{}: {}",
                    blasphemy::details::UNEXPECTED_TOKEN,
                    util::get_token_display_name(*tokens.front())
                ),
                blasphemy::blasphemy_type::SYNTAX,
                tokens.empty() ? blasphemy::NO_CODE_POS : tokens.front()->get_code_position()
            );
        }
    }
}
