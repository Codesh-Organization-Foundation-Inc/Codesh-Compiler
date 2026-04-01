#include "arithmetic_value_parser.h"

#include "value_parser.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "parser/ast/operator/math/addition_operator_ast_node.h"
#include "parser/ast/operator/math/division_operator_ast_node.h"
#include "parser/ast/operator/math/minus_operator_ast_node.h"
#include "parser/ast/operator/math/modulu_operator_ast_node.h"
#include "parser/ast/operator/math/multiplication_operator_ast_node.h"
#include "parser/ast/operator/math/subtraction_operator_ast_node.h"
#include "parser/ast/var_reference/error_value_ast_node.h"
#include "parser/util.h"
#include "token/token.h"

std::unique_ptr<codesh::ast::var_reference::value_ast_node> codesh::parser::value::parse_arithmetic_value(
        std::queue<std::unique_ptr<token>> &tokens)
{
    std::unique_ptr<ast::var_reference::value_ast_node> eval_ast_node;

    switch (tokens.front()->get_group())
    {
    case token_group::OPERATOR_ADDITION: {
        auto op_pos = tokens.front()->get_code_position();
        tokens.pop();

        auto left_value_node = parse_value(tokens);

        if (!util::consume_the(tokens))
        {
            return std::make_unique<ast::var_reference::error_value_ast_node>(op_pos);
        }

        auto right_value_node = parse_value(tokens);

        eval_ast_node = std::make_unique<ast::op::addition_operator_ast_node>(
            op_pos,
            std::move(left_value_node),
            std::move(right_value_node)
        );

        break;
    }

    case token_group::OPERATOR_SUBTRACTION: {
        auto op_pos = tokens.front()->get_code_position();
        tokens.pop();

        auto left_value_node = parse_value(tokens);

        if (!util::consume_the(tokens))
        {
            return std::make_unique<ast::var_reference::error_value_ast_node>(op_pos);
        }

        auto right_value_node = parse_value(tokens);

        eval_ast_node = std::make_unique<ast::op::subtraction_operator_ast_node>(
            op_pos,
            std::move(left_value_node),
            std::move(right_value_node)
        );

        break;
    }

    case token_group::OPERATOR_MULTIPLICATION: {
        auto op_pos = tokens.front()->get_code_position();
        tokens.pop();

        auto left_value_node = parse_value(tokens);

        if (!util::consume_by(tokens))
        {
            return std::make_unique<ast::var_reference::error_value_ast_node>(op_pos);
        }

        auto right_value_node = parse_value(tokens);

        eval_ast_node = std::make_unique<ast::op::multiplication_operator_ast_node>(
            op_pos,
            std::move(left_value_node),
            std::move(right_value_node)
        );

        break;
    }

    case token_group::OPERATOR_DIVISION: {
        auto op_pos = tokens.front()->get_code_position();
        tokens.pop();

        auto left_value_node = parse_value(tokens);

        if (!util::consume_by(tokens))
        {
            return std::make_unique<ast::var_reference::error_value_ast_node>(op_pos);
        }

        auto right_value_node = parse_value(tokens);

        eval_ast_node = std::make_unique<ast::op::division_operator_ast_node>(
            op_pos,
            std::move(left_value_node),
            std::move(right_value_node)
        );

        break;
    }

    case token_group::OPERATOR_MODULO: {
        auto op_pos = tokens.front()->get_code_position();
        tokens.pop();

        auto left_value_node  = parse_value(tokens);

        if (!util::consume_by(tokens))
        {
            return std::make_unique<ast::var_reference::error_value_ast_node>(op_pos);
        }

        auto right_value_node = parse_value(tokens);

        eval_ast_node = std::make_unique<ast::op::modulu_operator_ast_node>(
            op_pos,
            std::move(left_value_node),
            std::move(right_value_node)
        );

        break;
    }

    case token_group::OPEN_PARENTHESIS: {
        tokens.pop();

        auto inner = parse_value(tokens);

        if (!util::consuming_check(tokens, token_group::CLOSE_PARENTHESIS)) {
            blasphemy::get_blasphemy_collector().add_blasphemy(
                blasphemy::details::NO_CLOSE_PARENTHESIS,
                blasphemy::blasphemy_type::SYNTAX,
                inner->get_code_position()
            );
        }

        eval_ast_node = std::move(inner);
        break;
    }

    case token_group::OPERATOR_MINUS: {
        auto op_pos = tokens.front()->get_code_position();
        tokens.pop();

        eval_ast_node = std::make_unique<ast::op::minus_operator_ast_node>(
            op_pos,
            parse_value(tokens)
        );

        break;
    }

    default:
        eval_ast_node = std::make_unique<ast::var_reference::error_value_ast_node>(tokens.front()->get_code_position());
        break;
    }

    return eval_ast_node;
}
