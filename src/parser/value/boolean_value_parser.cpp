#include "boolean_value_parser.h"

#include "value_parser.h"

#include "util.h"
#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "parser/ast/operator/boolean/equals_operator_ast_node.h"
#include "parser/ast/operator/boolean/greater_equals_operator_ast_node.h"
#include "parser/ast/operator/boolean/greater_operator_ast_node.h"
#include "parser/ast/operator/boolean/less_equals_operator_ast_node.h"
#include "parser/ast/operator/boolean/less_operator_ast_node.h"
#include "parser/ast/operator/boolean/not_equals_operator_ast_node.h"
#include "parser/ast/var_reference/error_value_ast_node.h"

#include "fmt/format.h"
#include "parser/util.h"
#include "token/token.h"
#include "token/token_group.h"

static bool consume_against(std::queue<std::unique_ptr<codesh::token>> &tokens);

std::unique_ptr<codesh::ast::var_reference::value_ast_node> codesh::parser::value::parse_boolean_value(
    std::queue<std::unique_ptr<token>> &tokens)
{
    std::unique_ptr<ast::var_reference::value_ast_node> eval_ast_node;

    switch (tokens.front()->get_group())
    {
    case token_group::OPERATOR_EQUALS: {
        auto op_pos = tokens.front()->get_code_position();
        tokens.pop();

        auto left_value_node = parse_value(tokens);

        if (!consume_against(tokens))
        {
            return std::make_unique<ast::var_reference::error_value_ast_node>(op_pos);
        }

        auto right_value_node = parse_value(tokens);

        eval_ast_node = std::make_unique<ast::op::equals_operator_ast_node>(
            op_pos,
            std::move(left_value_node),
            std::move(right_value_node)
        );

        break;
    }
    case token_group::OPERATOR_NOT_EQUALS: {
        auto op_pos = tokens.front()->get_code_position();
        tokens.pop();

        auto left_value_node = parse_value(tokens);

        if (!consume_against(tokens))
        {
            return std::make_unique<ast::var_reference::error_value_ast_node>(op_pos);
        }

        auto right_value_node = parse_value(tokens);

        eval_ast_node = std::make_unique<ast::op::not_equals_operator_ast_node>(
            op_pos,
            std::move(left_value_node),
            std::move(right_value_node)
        );

        break;
    }
    case token_group::OPERATOR_GREATER: {
        auto op_pos = tokens.front()->get_code_position();
        tokens.pop();

        auto left_value_node = parse_value(tokens);

        if (!consume_against(tokens))
        {
            return std::make_unique<ast::var_reference::error_value_ast_node>(op_pos);
        }

        auto right_value_node = parse_value(tokens);

        eval_ast_node = std::make_unique<ast::op::greater_operator_ast_node>(
            op_pos,
            std::move(left_value_node),
            std::move(right_value_node)
        );

        break;
    }
    case token_group::OPERATOR_GREATER_EQUALS: {
        auto op_pos = tokens.front()->get_code_position();
        tokens.pop();

        auto left_value_node = parse_value(tokens);

        if (!consume_against(tokens))
        {
            return std::make_unique<ast::var_reference::error_value_ast_node>(op_pos);
        }

        auto right_value_node = parse_value(tokens);

        eval_ast_node = std::make_unique<ast::op::greater_equals_operator_ast_node>(
            op_pos,
            std::move(left_value_node),
            std::move(right_value_node)
        );

        break;
    }
    case token_group::OPERATOR_LESS: {
        auto op_pos = tokens.front()->get_code_position();
        tokens.pop();

        auto left_value_node = parse_value(tokens);

        if (!consume_against(tokens))
        {
            return std::make_unique<ast::var_reference::error_value_ast_node>(op_pos);
        }

        auto right_value_node = parse_value(tokens);

        eval_ast_node = std::make_unique<ast::op::less_operator_ast_node>(
            op_pos,
            std::move(left_value_node),
            std::move(right_value_node)
        );

        break;
    }
    case token_group::OPERATOR_LESS_EQUALS: {
        auto op_pos = tokens.front()->get_code_position();
        tokens.pop();

        auto left_value_node = parse_value(tokens);

        if (!consume_against(tokens))
        {
            return std::make_unique<ast::var_reference::error_value_ast_node>(op_pos);
        }

        auto right_value_node = parse_value(tokens);

        eval_ast_node = std::make_unique<ast::op::less_equals_operator_ast_node>(
            op_pos,
            std::move(left_value_node),
            std::move(right_value_node)
        );

        break;
    }
    default: {
        eval_ast_node = std::make_unique<ast::var_reference::error_value_ast_node>(tokens.front()->get_code_position());
        break;
    }
    }

    return eval_ast_node;
}

static bool consume_against(std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    if (!codesh::parser::util::consuming_check(tokens, codesh::token_group::OPERATOR_AGAINST)) {
        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
            codesh::blasphemy::details::NO_KEYWORD_AGAINST,
            codesh::blasphemy::blasphemy_type::SYNTAX,
            tokens.empty() ? codesh::lexer::NO_CODE_POS : tokens.front()->get_code_position()
        );

        return false;
    }

    return true;
}
