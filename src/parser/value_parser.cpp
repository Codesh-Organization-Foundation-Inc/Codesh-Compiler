#include "value_parser.h"

#include "../blasphemy/blasphemy_collector.h"
#include "../blasphemy/details.h"
#include "../defenition/primitive_type.h"
#include "ast/operator/boolean/and_operator_ast_node.h"
#include "ast/operator/boolean/equals_operator_ast_node.h"
#include "ast/operator/boolean/greater_equals_operator_ast_node.h"
#include "ast/operator/boolean/greater_operator_ast_node.h"
#include "ast/operator/boolean/less_equals_operator_ast_node.h"
#include "ast/operator/boolean/less_operator_ast_node.h"
#include "ast/operator/boolean/not_equals_operator_ast_node.h"
#include "ast/operator/boolean/not_operator_ast_node.h"
#include "ast/operator/boolean/or_operator_ast_node.h"
#include "ast/operator/math/addition_operator_ast_node.h"
#include "ast/operator/math/division_operator_ast_node.h"
#include "ast/operator/math/minus_operator_ast_node.h"
#include "ast/operator/math/modulu_operator_ast_node.h"
#include "ast/operator/math/multiplication_operator_ast_node.h"
#include "ast/operator/math/subtraction_operator_ast_node.h"
#include "ast/type/custom_type_ast_node.h"
#include "ast/type/primitive_type_ast_node.h"
#include "ast/var_reference/error_value_ast_node.h"
#include "ast/var_reference/evaluable_ast_node.h"
#include "ast/var_reference/variable_reference_ast_node.h"
#include "util.h"

#include <functional>

template <typename T>
static std::unique_ptr<codesh::ast::var_reference::evaluable_ast_node<T>> make_evaluable(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::definition::primitive_type primitive_type,
        const std::function<T(const std::string &)> &mapper);

template <typename T>
static std::unique_ptr<codesh::ast::var_reference::evaluable_ast_node<T>> make_evaluable(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::definition::primitive_type primitive_type,
        T value);

static std::unique_ptr<codesh::ast::var_reference::evaluable_ast_node<bool>> make_bool_evaluable(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        bool value);

static std::unique_ptr<codesh::ast::var_reference::value_ast_node> check_extras(
    std::queue<std::unique_ptr<codesh::token>> &tokens,
    std::unique_ptr<codesh::ast::var_reference::value_ast_node> eval_ast_node
);

static bool consume_against(std::queue<std::unique_ptr<codesh::token>> &tokens);

std::unique_ptr<codesh::ast::var_reference::value_ast_node> codesh::parser::parse_value(
        std::queue<std::unique_ptr<token>> &tokens)
{
    std::unique_ptr<ast::var_reference::value_ast_node> eval_ast_node;

    switch (tokens.front()->get_group())
    {
    case token_group::IDENTIFIER: {
        definition::fully_qualified_class_name value;
        util::parse_fqcn(tokens, value);

        eval_ast_node = std::make_unique<variable_reference_ast_node>(value);

        break;
    }

    case token_group::LITERAL_STRING: {
        eval_ast_node = std::make_unique<ast::var_reference::evaluable_ast_node<std::string>>(
            std::make_unique<ast::type::custom_type_ast_node>("java/lang/String"),

            util::consume_alnum_identifier_token(
                tokens, "לא אמור לקרות"
            )->get_content()
        );

        break;
    }

    case token_group::LITERAL_NUMBER_INT: {
        eval_ast_node = make_evaluable<int>(
            tokens, definition::primitive_type::INTEGER,

            [](const std::string &content) {
                return std::stoi(content);
            }
        );

        break;
    }

    case token_group::LITERAL_NUMBER_FLOAT: {
        eval_ast_node = make_evaluable<float>(
            tokens, definition::primitive_type::FLOAT,

            [](const std::string &content) {
                return std::stof(content);
            }
        );

        break;
    }

    case token_group::LITERAL_NUMBER_DOUBLE: {
        eval_ast_node = make_evaluable<double>(
            tokens, definition::primitive_type::DOUBLE,

            [](const std::string &content) {
                return std::stod(content);
            }
        );

        break;
    }

    case token_group::LITERAL_CHAR: {
        eval_ast_node = make_evaluable<char>(
            tokens, definition::primitive_type::CHAR,

            [](const std::string &content) {
                return content[0];
            }
        );

        break;
    }

    case token_group::KEYWORD_TRUE: {
        eval_ast_node = make_bool_evaluable(tokens, true);

        break;
    }

    case token_group::KEYWORD_FALSE: {
        eval_ast_node = make_bool_evaluable(tokens, false);

        break;
    }

    // Math operations:

    case token_group::OPERATOR_ADDITION: {
        tokens.pop();

        auto left_value_node = parse_value(tokens);
        auto right_value_node = parse_value(tokens);

        eval_ast_node = std::make_unique<ast::op::addition_operator_ast_node>(
            std::move(left_value_node),
            std::move(right_value_node)
        );

        break;
    }

    case token_group::OPERATOR_SUBTRACTION: {
        tokens.pop();

        auto left_value_node = parse_value(tokens);
        auto right_value_node = parse_value(tokens);

        eval_ast_node = std::make_unique<ast::op::subtraction_operator_ast_node>(
            std::move(left_value_node),
            std::move(right_value_node)
        );

        break;
    }

    case token_group::OPERATOR_MULTIPLICATION: {
        tokens.pop();

        auto left_value_node = parse_value(tokens);

        if (!util::consuming_check(tokens, token_group::OPERATOR_BY)) {
            blasphemy::get_blasphemy_collector().add_blasphemy(
                blasphemy::details::NO_KEYWORD_BY,
                blasphemy::blasphemy_type::SYNTAX
            );
        }

        auto right_value_node = parse_value(tokens);

        eval_ast_node = std::make_unique<ast::op::multiplication_operator_ast_node>(
            std::move(left_value_node),
            std::move(right_value_node)
        );

        break;
    }

    case token_group::OPERATOR_DIVISION: {
        tokens.pop();

        auto left_value_node = parse_value(tokens);

        if (!util::consuming_check(tokens, token_group::OPERATOR_BY)) {
            blasphemy::get_blasphemy_collector().add_blasphemy(
                blasphemy::details::NO_KEYWORD_BY,
                blasphemy::blasphemy_type::SYNTAX
            );
        }

        auto right_value_node = parse_value(tokens);

        eval_ast_node = std::make_unique<ast::op::division_operator_ast_node>(
            std::move(left_value_node),
            std::move(right_value_node)
        );

        break;
    }

    case token_group::OPERATOR_MODULO: {
        tokens.pop();

        auto left_value_node  = parse_value(tokens);

        if (!util::consuming_check(tokens, token_group::OPERATOR_BY)) {
            blasphemy::get_blasphemy_collector().add_blasphemy(
                blasphemy::details::NO_KEYWORD_BY,
                blasphemy::blasphemy_type::SYNTAX
            );
        }

        auto right_value_node = parse_value(tokens);

        eval_ast_node = std::make_unique<ast::op::modulu_operator_ast_node>(
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
                blasphemy::blasphemy_type::SYNTAX
            );
        }

        eval_ast_node = std::move(inner);
        break;
    }

    case token_group::OPERATOR_MINUS: {
        tokens.pop();

        eval_ast_node = std::make_unique<ast::op::minus_operator_ast_node>(
            parse_value(tokens)
        );

        break;
    }

    case token_group::OPERATOR_EQUALS:
    case token_group::OPERATOR_NOT_EQUALS:
    case token_group::OPERATOR_GREATER:
    case token_group::OPERATOR_GREATER_EQUALS:
    case token_group::OPERATOR_LESS:
    case token_group::OPERATOR_LESS_EQUALS:
        eval_ast_node = parse_boolean_value(tokens);
        break;

    default: {
        eval_ast_node = std::make_unique<ast::var_reference::error_value_ast_node>(); // FIXME: does problems
        tokens.pop();

        blasphemy::get_blasphemy_collector().add_blasphemy(
            blasphemy::details::UNEXPECTED_TOKEN,
            blasphemy::blasphemy_type::SYNTAX
        );
    }
    }

    return check_extras(tokens, std::move(eval_ast_node));
}

std::unique_ptr<codesh::ast::var_reference::value_ast_node> codesh::parser::parse_boolean_value(
    std::queue<std::unique_ptr<token>> &tokens)
{
    std::unique_ptr<ast::var_reference::value_ast_node> eval_ast_node;

    switch (tokens.front()->get_group())
    {
    case token_group::OPERATOR_EQUALS: {
        tokens.pop();

        auto left_value_node  = parse_value(tokens);

        if (!consume_against(tokens))
        {
            return std::make_unique<ast::var_reference::error_value_ast_node>();
        }

        auto right_value_node = parse_value(tokens);

        eval_ast_node = std::make_unique<ast::op::equals_operator_ast_node>(
            std::move(left_value_node),
            std::move(right_value_node)
        );

        break;
    }
    case token_group::OPERATOR_NOT_EQUALS: {
        tokens.pop();

        auto left_value_node  = parse_value(tokens);

        if (!consume_against(tokens))
        {
            return std::make_unique<ast::var_reference::error_value_ast_node>();
        }

        auto right_value_node = parse_value(tokens);

        eval_ast_node = std::make_unique<ast::op::not_equals_operator_ast_node>(
            std::move(left_value_node),
            std::move(right_value_node)
        );

        break;
    }
    case token_group::OPERATOR_GREATER: {
        tokens.pop();

        auto left_value_node  = parse_value(tokens);

        if (!consume_against(tokens))
        {
            return std::make_unique<ast::var_reference::error_value_ast_node>();
        }

        auto right_value_node = parse_value(tokens);

        eval_ast_node = std::make_unique<ast::op::greater_operator_ast_node>(
            std::move(left_value_node),
            std::move(right_value_node)
        );

        break;
    }
    case token_group::OPERATOR_GREATER_EQUALS: {
        tokens.pop();

        auto left_value_node  = parse_value(tokens);

        if (!consume_against(tokens))
        {
            return std::make_unique<ast::var_reference::error_value_ast_node>();
        }

        auto right_value_node = parse_value(tokens);

        eval_ast_node = std::make_unique<ast::op::greater_equals_operator_ast_node>(
            std::move(left_value_node),
            std::move(right_value_node)
        );

        break;
    }
    case token_group::OPERATOR_LESS: {
        tokens.pop();

        auto left_value_node  = parse_value(tokens);

        if (!consume_against(tokens))
        {
            return std::make_unique<ast::var_reference::error_value_ast_node>();
        }

        auto right_value_node = parse_value(tokens);

        eval_ast_node = std::make_unique<ast::op::less_operator_ast_node>(
            std::move(left_value_node),
            std::move(right_value_node)
        );

        break;
    }
    case token_group::OPERATOR_LESS_EQUALS: {
        tokens.pop();

        auto left_value_node  = parse_value(tokens);

        if (!consume_against(tokens))
        {
            return std::make_unique<ast::var_reference::error_value_ast_node>();
        }

        auto right_value_node = parse_value(tokens);

        eval_ast_node = std::make_unique<ast::op::less_equals_operator_ast_node>(
            std::move(left_value_node),
            std::move(right_value_node)
        );

        break;
    }
    default: {
        eval_ast_node = std::make_unique<ast::var_reference::error_value_ast_node>(); // FIXME: does problems
        tokens.pop();

        blasphemy::get_blasphemy_collector().add_blasphemy(
            blasphemy::details::UNEXPECTED_TOKEN,
            blasphemy::blasphemy_type::SYNTAX
        );
    }
    }
    return eval_ast_node;
}

static std::unique_ptr<codesh::ast::var_reference::value_ast_node> check_extras(
    std::queue<std::unique_ptr<codesh::token>> &tokens,
    std::unique_ptr<codesh::ast::var_reference::value_ast_node> eval_ast_node
)
{
    switch (tokens.front()->get_group())
    {
        case codesh::token_group::OPERATOR_NOT: {
            tokens.pop();
            return std::make_unique<codesh::ast::op::not_operator_ast_node>(std::move(eval_ast_node));
        }
        case codesh::token_group::OPERATOR_AND: {
            tokens.pop();
            return std::make_unique<codesh::ast::op::and_operator_ast_node>(std::move(eval_ast_node),
                codesh::parser::parse_value(tokens));
        }
        case codesh::token_group::OPERATOR_OR: {
            tokens.pop();
            return std::make_unique<codesh::ast::op::or_operator_ast_node>(std::move(eval_ast_node),
                codesh::parser::parse_value(tokens));
        }
        default: {
            return eval_ast_node;
        }
    }

}

static bool consume_against(std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    if (!codesh::parser::util::consuming_check(tokens, codesh::token_group::OPERATOR_AGAINST)) {
        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
            codesh::blasphemy::details::NO_KEYWORD_AGAINST,
            codesh::blasphemy::blasphemy_type::SYNTAX
        );

        return false;
    }

    return true;
}

template <typename T>
static std::unique_ptr<codesh::ast::var_reference::evaluable_ast_node<T>> make_evaluable(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::definition::primitive_type primitive_type,
        const std::function<T(const std::string &)> &mapper)
{
    return std::make_unique<codesh::ast::var_reference::evaluable_ast_node<T>>(
        std::make_unique<codesh::ast::type::primitive_type_ast_node>(primitive_type),
        mapper(
            codesh::parser::util::consume_alnum_identifier_token(
                tokens, "לא אמור לקרות"
            )->get_content()
        )
    );
}

template <typename T>
static std::unique_ptr<codesh::ast::var_reference::evaluable_ast_node<T>> make_evaluable(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::definition::primitive_type primitive_type,
        T value)
{
    tokens.pop();

    return std::make_unique<codesh::ast::var_reference::evaluable_ast_node<T>>(
        std::make_unique<codesh::ast::type::primitive_type_ast_node>(primitive_type),
        std::move(value)
    );
}

static std::unique_ptr<codesh::ast::var_reference::evaluable_ast_node<bool>> make_bool_evaluable(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        const bool value)
{
    return make_evaluable<bool>(
        tokens,
        codesh::definition::primitive_type::BOOLEAN,
        value
    );
}