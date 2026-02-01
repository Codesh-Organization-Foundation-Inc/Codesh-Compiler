#include "value_parser.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "defenition/primitive_type.h"
#include "parser/ast/collection/range_ast_node.h"
#include "parser/ast/operator/assignment/addition_assignment_operator_ast_node.h"
#include "parser/ast/operator/assignment/division_assignment_operator_ast_node.h"
#include "parser/ast/operator/assignment/modulo_assignment_operator_ast_node.h"
#include "parser/ast/operator/assignment/multiplication_assignment_operator_ast_node.h"
#include "parser/ast/operator/assignment/subtraction_assignment_operator_ast_node.h"
#include "parser/ast/operator/boolean/and_operator_ast_node.h"
#include "parser/ast/operator/boolean/equals_operator_ast_node.h"
#include "parser/ast/operator/boolean/greater_equals_operator_ast_node.h"
#include "parser/ast/operator/boolean/greater_operator_ast_node.h"
#include "parser/ast/operator/boolean/less_equals_operator_ast_node.h"
#include "parser/ast/operator/boolean/less_operator_ast_node.h"
#include "parser/ast/operator/boolean/not_equals_operator_ast_node.h"
#include "parser/ast/operator/boolean/not_operator_ast_node.h"
#include "parser/ast/operator/boolean/or_operator_ast_node.h"
#include "parser/ast/operator/math/addition_operator_ast_node.h"
#include "parser/ast/operator/math/division_operator_ast_node.h"
#include "parser/ast/operator/math/minus_operator_ast_node.h"
#include "parser/ast/operator/math/modulu_operator_ast_node.h"
#include "parser/ast/operator/math/multiplication_operator_ast_node.h"
#include "parser/ast/operator/math/subtraction_operator_ast_node.h"
#include "parser/ast/type/custom_type_ast_node.h"
#include "parser/ast/type/primitive_type_ast_node.h"
#include "parser/ast/var_reference/error_value_ast_node.h"
#include "parser/ast/var_reference/evaluable_ast_node.h"
#include "parser/ast/var_reference/variable_reference_ast_node.h"
#include "util.h"

#include "fmt/format.h"

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
        std::unique_ptr<codesh::ast::var_reference::value_ast_node> eval_ast_node);

static std::unique_ptr<codesh::ast::collection::range_ast_node> parse_range(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        std::unique_ptr<codesh::ast::var_reference::value_ast_node> eval_ast_node);

static bool consume_against(std::queue<std::unique_ptr<codesh::token>> &tokens);

static bool consume_by(std::queue<std::unique_ptr<codesh::token>> &tokens);

/**
 * @param tokens The queue of tokens
 * @param sepd_by_by Whether the operands should be separated by the "by" operator
 * @returns The lhs & rhs of the operator, or @link std::nullopt \endlink upon failure
 */
static std::optional<std::pair<
    std::unique_ptr<variable_reference_ast_node>,
    std::unique_ptr<codesh::ast::var_reference::value_ast_node>
>> parse_operator_sides(std::queue<std::unique_ptr<codesh::token>> &tokens, bool sepd_by_by);

std::unique_ptr<codesh::ast::var_reference::value_ast_node> codesh::parser::parse_value(
        std::queue<std::unique_ptr<token>> &tokens)
{
    std::unique_ptr<ast::var_reference::value_ast_node> eval_ast_node;

    switch (tokens.front()->get_group())
    {
    case token_group::IDENTIFIER: {
        auto id_pos = tokens.front()->get_code_position();

        definition::fully_qualified_name value;
        util::parse_fqcn(tokens, value);

        eval_ast_node = std::make_unique<variable_reference_ast_node>(id_pos, value);

        break;
    }

    case token_group::LITERAL_STRING: {
        auto str_pos = tokens.front()->get_code_position();

        eval_ast_node = std::make_unique<ast::var_reference::evaluable_ast_node<std::string>>(
            str_pos,
            std::make_unique<ast::type::custom_type_ast_node>(str_pos, "java/lang/String"),

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
        auto op_pos = tokens.front()->get_code_position();
        tokens.pop();

        auto left_value_node = parse_value(tokens);
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

        if (!consume_by(tokens))
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

        if (!consume_by(tokens))
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

        if (!consume_by(tokens))
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

    case token_group::OPERATOR_EQUALS:
    case token_group::OPERATOR_NOT_EQUALS:
    case token_group::OPERATOR_GREATER:
    case token_group::OPERATOR_GREATER_EQUALS:
    case token_group::OPERATOR_LESS:
    case token_group::OPERATOR_LESS_EQUALS:
        eval_ast_node = parse_boolean_value(tokens);
        break;

    case token_group::OPERATOR_ADDITION_ASSIGNMENT:
    case token_group::OPERATOR_DIVISION_ASSIGNMENT:
    case token_group::OPERATOR_MODULO_ASSIGNMENT:
    case token_group::OPERATOR_MULTIPLICATION_ASSIGNMENT:
    case token_group::OPERATOR_SUBTRACTION_ASSIGNMENT:
        eval_ast_node = parse_assignment_operator(tokens);
        break;

    default: {
        const auto error_pos = tokens.front()->get_code_position();
        const auto token_name = util::get_token_display_name(*tokens.front());
        eval_ast_node = std::make_unique<ast::var_reference::error_value_ast_node>(error_pos);
        tokens.pop();

        blasphemy::get_blasphemy_collector().add_blasphemy(
            fmt::format(
                "{}: {}",
                blasphemy::details::UNEXPECTED_TOKEN,
                token_name
            ),
            blasphemy::blasphemy_type::SYNTAX,
            error_pos
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
        const auto error_pos = tokens.front()->get_code_position();
        const auto token_name = util::get_token_display_name(*tokens.front());
        eval_ast_node = std::make_unique<ast::var_reference::error_value_ast_node>(error_pos); // FIXME: does problems
        tokens.pop();

        blasphemy::get_blasphemy_collector().add_blasphemy(
            fmt::format(
                "{}: {}",
                blasphemy::details::UNEXPECTED_TOKEN,
                token_name
            ),
            blasphemy::blasphemy_type::SYNTAX,
            error_pos
        );
    }
    }

    return eval_ast_node;
}

static std::unique_ptr<codesh::ast::collection::range_ast_node> parse_range(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        std::unique_ptr<codesh::ast::var_reference::value_ast_node> eval_ast_node)
{
    auto to_val = codesh::parser::parse_value(tokens);

    std::unique_ptr<codesh::ast::var_reference::value_ast_node> skip_val = nullptr;
    if (codesh::parser::util::consuming_check(tokens, codesh::token_group::KEYWORD_SKIP))
    {
        skip_val = codesh::parser::parse_value(tokens);
    }
    else
    {
        skip_val = std::make_unique<codesh::ast::var_reference::evaluable_ast_node<int>>(
            codesh::blasphemy::NO_CODE_POS,
            std::make_unique<codesh::ast::type::primitive_type_ast_node>(codesh::blasphemy::NO_CODE_POS, codesh::definition::primitive_type::INTEGER),
            1
        );
    }

    return std::make_unique<codesh::ast::collection::range_ast_node>(
        codesh::blasphemy::NO_CODE_POS,
        std::move(eval_ast_node),
        std::move(to_val),
        std::move(skip_val)
    );
}


std::unique_ptr<codesh::ast::var_reference::value_ast_node> codesh::parser::parse_assignment_operator(
    std::queue<std::unique_ptr<token>> &tokens)
{
    std::unique_ptr<ast::var_reference::value_ast_node> eval_ast_node;

    switch (tokens.front()->get_group())
    {
    case token_group::OPERATOR_ADDITION_ASSIGNMENT: {
        auto op_pos = tokens.front()->get_code_position();
        tokens.pop();

        auto result = parse_operator_sides(tokens, false);
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

        auto result = parse_operator_sides(tokens, false);
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

        auto result = parse_operator_sides(tokens, true);
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

        auto result = parse_operator_sides(tokens, true);
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

        auto result = parse_operator_sides(tokens, true);
        if (!result.has_value())
            return std::make_unique<ast::var_reference::error_value_ast_node>(op_pos);

        eval_ast_node = std::make_unique<ast::op::assignment::multiplication_assignment_operator_ast_node>(
            op_pos,
            std::move(result->first),
            std::move(result->second)
        );
        break;
    }
    default: {
        std::make_unique<ast::var_reference::error_value_ast_node>(tokens.front()->get_code_position());
    }
    }
    return eval_ast_node;
}

static std::optional<std::pair<
    std::unique_ptr<variable_reference_ast_node>,
    std::unique_ptr<codesh::ast::var_reference::value_ast_node>
>> parse_operator_sides(std::queue<std::unique_ptr<codesh::token>> &tokens, const bool sepd_by_by)
{
    auto left_value_node  = codesh::parser::parse_value(tokens);
    if (sepd_by_by && !consume_by(tokens))
        return std::nullopt;

    auto right_value_node = codesh::parser::parse_value(tokens);

    // Left side must be a variable
    if (!dynamic_cast<variable_reference_ast_node *>(left_value_node.get()))
    {
        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
            codesh::blasphemy::details::EXPECTED_VARIABLE,
            codesh::blasphemy::blasphemy_type::SYNTAX,
            left_value_node->get_code_position()
        );

        return std::nullopt;
    }

    return std::pair {
        std::unique_ptr<variable_reference_ast_node>(
            static_cast<variable_reference_ast_node *>(left_value_node.release()) // NOLINT(*-pro-type-static-cast-downcast)
        ),
        std::move(right_value_node)
    };
}

static std::unique_ptr<codesh::ast::var_reference::value_ast_node> check_extras(
    std::queue<std::unique_ptr<codesh::token>> &tokens,
    std::unique_ptr<codesh::ast::var_reference::value_ast_node> eval_ast_node
)
{
    switch (tokens.front()->get_group())
    {
        case codesh::token_group::KEYWORD_TO: {
            tokens.pop();
            return parse_range(tokens, std::move(eval_ast_node));
        }

        case codesh::token_group::OPERATOR_NOT: {
            auto op_pos = tokens.front()->get_code_position();
            tokens.pop();

            return std::make_unique<codesh::ast::op::not_operator_ast_node>(op_pos, std::move(eval_ast_node));
        }
        case codesh::token_group::OPERATOR_AND: {
            auto op_pos = tokens.front()->get_code_position();
            tokens.pop();

            return std::make_unique<codesh::ast::op::and_operator_ast_node>(op_pos, std::move(eval_ast_node),
                codesh::parser::parse_value(tokens));
        }
        case codesh::token_group::OPERATOR_OR: {
            auto op_pos = tokens.front()->get_code_position();
            tokens.pop();

            return std::make_unique<codesh::ast::op::or_operator_ast_node>(op_pos, std::move(eval_ast_node),
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
            codesh::blasphemy::blasphemy_type::SYNTAX,
            tokens.empty() ? codesh::blasphemy::NO_CODE_POS : tokens.front()->get_code_position()
        );

        return false;
    }

    return true;
}

static bool consume_by(std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    if (!codesh::parser::util::consuming_check(tokens, codesh::token_group::OPERATOR_BY)) {
        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
            codesh::blasphemy::details::NO_KEYWORD_BY,
            codesh::blasphemy::blasphemy_type::SYNTAX,
            tokens.empty() ? codesh::blasphemy::NO_CODE_POS : tokens.front()->get_code_position()
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
    auto pos = tokens.front()->get_code_position();

    return std::make_unique<codesh::ast::var_reference::evaluable_ast_node<T>>(
        pos,
        std::make_unique<codesh::ast::type::primitive_type_ast_node>(pos, primitive_type),
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
    auto pos = tokens.front()->get_code_position();
    tokens.pop();

    return std::make_unique<codesh::ast::var_reference::evaluable_ast_node<T>>(
        pos,
        std::make_unique<codesh::ast::type::primitive_type_ast_node>(pos, primitive_type),
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
