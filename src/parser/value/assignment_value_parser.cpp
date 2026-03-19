#include "assignment_value_parser.h"

#include "value_parser.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "parser/ast/operator/assignment/addition_assignment_operator_ast_node.h"
#include "parser/ast/operator/assignment/decrement_operator_ast_node.h"
#include "parser/ast/operator/assignment/division_assignment_operator_ast_node.h"
#include "parser/ast/operator/assignment/increment_operator_ast_node.h"
#include "parser/ast/operator/assignment/modulo_assignment_operator_ast_node.h"
#include "parser/ast/operator/assignment/multiplication_assignment_operator_ast_node.h"
#include "parser/ast/operator/assignment/subtraction_assignment_operator_ast_node.h"
#include "parser/ast/var_reference/error_value_ast_node.h"
#include "parser/util.h"
#include "parser/ast/operator/assignment/assign_operator_ast_node.h"
#include "token/token.h"
#include "token/token_group.h"

/**
 * @param tokens The queue of tokens
 * @param sepd_by_by Are the operands separated by the "by" operator or by the "the" operator
 * @returns The lhs & rhs of the operator, or @c std::nullopt upon failure
 */
static std::optional<std::pair<
    std::unique_ptr<codesh::ast::var_reference::variable_reference_ast_node>,
    std::unique_ptr<codesh::ast::var_reference::value_ast_node>
>> parse_assignment_operator_sides(std::queue<std::unique_ptr<codesh::token>> &tokens, bool sepd_by_by);

/**
 * Parses the next value from @p tokens and validates it is a variable reference.
 * @returns The variable node on success, or @c nullptr (after reporting a blasphemy) on failure.
 */
static std::unique_ptr<codesh::ast::var_reference::variable_reference_ast_node> parse_variable(std::queue<std::unique_ptr<codesh::token>> &tokens);

std::unique_ptr<codesh::ast::var_reference::value_ast_node> codesh::parser::value::parse_assignment_operator(
    std::queue<std::unique_ptr<token>> &tokens)
{
    std::unique_ptr<ast::var_reference::value_ast_node> eval_ast_node;

    switch (tokens.front()->get_group())
    {
    case token_group::KEYWORD_REPLACE: {
        auto op_pos = tokens.front()->get_code_position();
        tokens.pop();

        // parse lhs
        auto left_value_node = parse_value(tokens);

        if (!util::consume_punc_equal(tokens))
            return std::make_unique<ast::var_reference::error_value_ast_node>(op_pos);

        // parse rhs
        auto right_value_node = parse_value(tokens);

        // lhs must be a variable
        if (!dynamic_cast<const codesh::ast::var_reference::variable_reference_ast_node *>(left_value_node.get()))
        {
            blasphemy::get_blasphemy_collector().add_blasphemy(
                blasphemy::details::EXPECTED_VARIABLE,
                blasphemy::blasphemy_type::SYNTAX,
                left_value_node->get_code_position()
            );

            return std::make_unique<ast::var_reference::error_value_ast_node>(op_pos);
        }

        eval_ast_node = std::make_unique<ast::op::assignment::assign_operator_ast_node>(
            op_pos,
            std::unique_ptr<codesh::ast::var_reference::variable_reference_ast_node>(
                static_cast<codesh::ast::var_reference::variable_reference_ast_node *>(left_value_node.release()) // NOLINT(*-pro-type-static-cast-downcast)
            ),
            std::move(right_value_node)
        );

        break;
    }
    case token_group::OPERATOR_ADDITION_ASSIGNMENT: {
        auto op_pos = tokens.front()->get_code_position();
        tokens.pop();

        auto result = parse_assignment_operator_sides(tokens, false);
        if (!result.has_value())
            return std::make_unique<ast::var_reference::error_value_ast_node>(op_pos);

        eval_ast_node = std::make_unique<ast::op::assignment::addition_assignment_operator_ast_node>(
            op_pos,
            std::move(result->first),
            std::move(result->second)
        );
        break;
    }
    case token_group::OPERATOR_SUBTRACTION_ASSIGNMENT: {
        auto op_pos = tokens.front()->get_code_position();
        tokens.pop();

        auto result = parse_assignment_operator_sides(tokens, false);
        if (!result.has_value())
            return std::make_unique<ast::var_reference::error_value_ast_node>(op_pos);

        eval_ast_node = std::make_unique<ast::op::assignment::subtraction_assignment_operator_ast_node>(
            op_pos,
            std::move(result->first),
            std::move(result->second)
        );
        break;
    }
    case token_group::OPERATOR_DIVISION_ASSIGNMENT: {
        auto op_pos = tokens.front()->get_code_position();
        tokens.pop();

        auto result = parse_assignment_operator_sides(tokens, true);
        if (!result.has_value())
            return std::make_unique<ast::var_reference::error_value_ast_node>(op_pos);

        eval_ast_node = std::make_unique<ast::op::assignment::division_assignment_operator_ast_node>(
            op_pos,
            std::move(result->first),
            std::move(result->second)
        );
        break;
    }
    case token_group::OPERATOR_MODULO_ASSIGNMENT: {
        auto op_pos = tokens.front()->get_code_position();
        tokens.pop();

        auto result = parse_assignment_operator_sides(tokens, true);
        if (!result.has_value())
            return std::make_unique<ast::var_reference::error_value_ast_node>(op_pos);

        eval_ast_node = std::make_unique<ast::op::assignment::modulo_assignment_operator_ast_node>(
            op_pos,
            std::move(result->first),
            std::move(result->second)
        );
        break;
    }
    case token_group::OPERATOR_MULTIPLICATION_ASSIGNMENT: {
        auto op_pos = tokens.front()->get_code_position();
        tokens.pop();

        auto result = parse_assignment_operator_sides(tokens, true);
        if (!result.has_value())
            return std::make_unique<ast::var_reference::error_value_ast_node>(op_pos);

        eval_ast_node = std::make_unique<ast::op::assignment::multiplication_assignment_operator_ast_node>(
            op_pos,
            std::move(result->first),
            std::move(result->second)
        );
        break;
    }
    case token_group::OPERATOR_INCREMENT: {
        auto op_pos = tokens.front()->get_code_position();
        tokens.pop();

        auto variable_node = parse_variable(tokens);
        if (!variable_node)
            return std::make_unique<ast::var_reference::error_value_ast_node>(op_pos);

        eval_ast_node = std::make_unique<ast::op::assignment::increment_operator_ast_node>(
            op_pos, std::move(variable_node)
        );
        break;
    }
    case token_group::OPERATOR_DECREMENT: {
        auto op_pos = tokens.front()->get_code_position();
        tokens.pop();

        auto variable_node = parse_variable(tokens);
        if (!variable_node)
            return std::make_unique<ast::var_reference::error_value_ast_node>(op_pos);

        eval_ast_node = std::make_unique<ast::op::assignment::decrement_operator_ast_node>(
            op_pos, std::move(variable_node)
        );
        break;
    }
    default: {
        eval_ast_node = std::make_unique<ast::var_reference::error_value_ast_node>(tokens.front()->get_code_position());
    }
    }
    return eval_ast_node;
}

static std::optional<std::pair<
    std::unique_ptr<codesh::ast::var_reference::variable_reference_ast_node>,
    std::unique_ptr<codesh::ast::var_reference::value_ast_node>
>> parse_assignment_operator_sides(std::queue<std::unique_ptr<codesh::token>> &tokens, const bool sepd_by_by)
{
    auto left_variable_node = parse_variable(tokens);
    if (!left_variable_node)
        return std::nullopt;

    if (sepd_by_by && !codesh::parser::util::consume_by(tokens))
        return std::nullopt;

    if (!sepd_by_by && !codesh::parser::util::consume_the(tokens))
        return std::nullopt;

    auto right_value_node = codesh::parser::value::parse_value(tokens);

    return std::pair { std::move(left_variable_node), std::move(right_value_node) };
}

static std::unique_ptr<codesh::ast::var_reference::variable_reference_ast_node> parse_variable(std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    auto value_node = codesh::parser::value::parse_value(tokens);

    if (auto *var = dynamic_cast<codesh::ast::var_reference::variable_reference_ast_node *>(value_node.get()))
    {
        value_node.release(); // NOLINT(*-unused-return-value)
        return std::unique_ptr<codesh::ast::var_reference::variable_reference_ast_node>(var);
    }

    codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
        codesh::blasphemy::details::EXPECTED_VARIABLE,
        codesh::blasphemy::blasphemy_type::SYNTAX,
        value_node->get_code_position()
    );
    return nullptr;
}
