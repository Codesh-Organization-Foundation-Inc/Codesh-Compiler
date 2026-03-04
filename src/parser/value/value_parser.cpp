#include "value_parser.h"

#include "arithmetic_value_parser.h"
#include "assignment_value_parser.h"
#include "biblical_numbers_parser.h"
#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "boolean_value_parser.h"
#include "defenition/primitive_type.h"
#include "new_value_parser.h"
#include "parser/ast/collection/range_ast_node.h"
#include "parser/ast/operator/boolean/and_operator_ast_node.h"
#include "parser/ast/operator/boolean/not_operator_ast_node.h"
#include "parser/ast/operator/boolean/or_operator_ast_node.h"
#include "parser/ast/type/primitive_type_ast_node.h"
#include "parser/ast/var_reference/error_value_ast_node.h"
#include "parser/ast/var_reference/evaluable_ast_node.h"
#include "primitive_value_parser.h"

#include "fmt/format.h"
#include "parser/type/class/method_parser.h"
#include "parser/util.h"
#include "token/token_group.h"

static std::unique_ptr<codesh::ast::var_reference::value_ast_node> check_extras(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        std::unique_ptr<codesh::ast::var_reference::value_ast_node> eval_ast_node);

static std::unique_ptr<codesh::ast::collection::range_ast_node> parse_range(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        std::unique_ptr<codesh::ast::var_reference::value_ast_node> eval_ast_node);

std::unique_ptr<codesh::ast::var_reference::value_ast_node> codesh::parser::value::parse_value(
        std::queue<std::unique_ptr<token>> &tokens)
{
    std::unique_ptr<ast::var_reference::value_ast_node> eval_ast_node;

    switch (tokens.front()->get_group())
    {
    // Primitives
    case token_group::KEYWORD_THIS:
    case token_group::IDENTIFIER:
    case token_group::LITERAL_STRING:
    case token_group::LITERAL_NUMBER_INT:
    case token_group::LITERAL_NUMBER_FLOAT:
    case token_group::LITERAL_NUMBER_DOUBLE:
    case token_group::LITERAL_CHAR:
    case token_group::KEYWORD_TRUE:
    case token_group::KEYWORD_FALSE:
        eval_ast_node = parse_primitive_value(tokens);
        break;

    // Biblical numbers
    case token_group::KEYWORD_BIBLICAL_DECIMAL_SEPARATOR:
    case token_group::KEYWORD_BIBLICAL_ZERO:
    case token_group::KEYWORD_BIBLICAL_ONE:
    case token_group::KEYWORD_BIBLICAL_TWO:
    case token_group::KEYWORD_BIBLICAL_THREE:
    case token_group::KEYWORD_BIBLICAL_FOUR:
    case token_group::KEYWORD_BIBLICAL_FIVE:
    case token_group::KEYWORD_BIBLICAL_SIX:
    case token_group::KEYWORD_BIBLICAL_SEVEN:
    case token_group::KEYWORD_BIBLICAL_EIGHT:
    case token_group::KEYWORD_BIBLICAL_NINE:
    case token_group::KEYWORD_BIBLICAL_TEN:
    case token_group::KEYWORD_BIBLICAL_TWELVE:
    case token_group::KEYWORD_BIBLICAL_TWENTY:
    case token_group::KEYWORD_BIBLICAL_THIRTY:
    case token_group::KEYWORD_BIBLICAL_FORTY:
    case token_group::KEYWORD_BIBLICAL_FIFTY:
    case token_group::KEYWORD_BIBLICAL_SIXTY:
    case token_group::KEYWORD_BIBLICAL_SEVENTY:
    case token_group::KEYWORD_BIBLICAL_EIGHTY:
    case token_group::KEYWORD_BIBLICAL_NINETY:
    case token_group::KEYWORD_BIBLICAL_HUNDRED:
    case token_group::KEYWORD_BIBLICAL_THOUSAND:
    case token_group::KEYWORD_BIBLICAL_TEN_THOUSAND:
    case token_group::KEYWORD_BIBLICAL_ZERO_ADDED:
    case token_group::KEYWORD_BIBLICAL_ONE_ADDED:
    case token_group::KEYWORD_BIBLICAL_TWO_ADDED:
    case token_group::KEYWORD_BIBLICAL_THREE_ADDED:
    case token_group::KEYWORD_BIBLICAL_FOUR_ADDED:
    case token_group::KEYWORD_BIBLICAL_FIVE_ADDED:
    case token_group::KEYWORD_BIBLICAL_SIX_ADDED:
    case token_group::KEYWORD_BIBLICAL_SEVEN_ADDED:
    case token_group::KEYWORD_BIBLICAL_EIGHT_ADDED:
    case token_group::KEYWORD_BIBLICAL_NINE_ADDED:
    case token_group::KEYWORD_BIBLICAL_TEN_ADDED:
    case token_group::KEYWORD_BIBLICAL_TWELVE_ADDED:
    case token_group::KEYWORD_BIBLICAL_TWENTY_ADDED:
    case token_group::KEYWORD_BIBLICAL_THIRTY_ADDED:
    case token_group::KEYWORD_BIBLICAL_FORTY_ADDED:
    case token_group::KEYWORD_BIBLICAL_FIFTY_ADDED:
    case token_group::KEYWORD_BIBLICAL_SIXTY_ADDED:
    case token_group::KEYWORD_BIBLICAL_SEVENTY_ADDED:
    case token_group::KEYWORD_BIBLICAL_EIGHTY_ADDED:
    case token_group::KEYWORD_BIBLICAL_NINETY_ADDED:
    case token_group::KEYWORD_BIBLICAL_HUNDRED_ADDED:
    case token_group::KEYWORD_BIBLICAL_THOUSAND_ADDED:
    case token_group::KEYWORD_BIBLICAL_TEN_THOUSAND_ADDED:
        eval_ast_node = biblical_numbers_parser(tokens).parse();
        break;

    // Arithmetic operations
    case token_group::OPERATOR_ADDITION:
    case token_group::OPERATOR_SUBTRACTION:
    case token_group::OPERATOR_MULTIPLICATION:
    case token_group::OPERATOR_DIVISION:
    case token_group::OPERATOR_MODULO:
    case token_group::OPEN_PARENTHESIS:
    case token_group::OPERATOR_MINUS:
        eval_ast_node = parse_arithmetic_value(tokens);
        break;

    // Boolean operations
    case token_group::OPERATOR_EQUALS:
    case token_group::OPERATOR_NOT_EQUALS:
    case token_group::OPERATOR_GREATER:
    case token_group::OPERATOR_GREATER_EQUALS:
    case token_group::OPERATOR_LESS:
    case token_group::OPERATOR_LESS_EQUALS:
        eval_ast_node = parse_boolean_value(tokens);
        break;

    // Assignment operations
    case token_group::KEYWORD_REPLACE:
    case token_group::OPERATOR_ADDITION_ASSIGNMENT:
    case token_group::OPERATOR_DIVISION_ASSIGNMENT:
    case token_group::OPERATOR_MODULO_ASSIGNMENT:
    case token_group::OPERATOR_MULTIPLICATION_ASSIGNMENT:
    case token_group::OPERATOR_SUBTRACTION_ASSIGNMENT:
    case token_group::OPERATOR_INCREMENT:
    case token_group::OPERATOR_DECREMENT:
        eval_ast_node = parse_assignment_operator(tokens);
        break;

    case token_group::KEYWORD_FUNCTION_CALL:
        eval_ast_node = parse_method_call(tokens);
        break;

    // New operator
    case token_group::KEYWORD_NEW:
        return parse_new_operator(tokens);

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
        break;
    }
    }

    return check_extras(tokens, std::move(eval_ast_node));
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
                codesh::parser::value::parse_value(tokens));
        }
        case codesh::token_group::OPERATOR_OR: {
            auto op_pos = tokens.front()->get_code_position();
            tokens.pop();

            return std::make_unique<codesh::ast::op::or_operator_ast_node>(op_pos, std::move(eval_ast_node),
                codesh::parser::value::parse_value(tokens));
        }
        default: {
            return eval_ast_node;
        }
    }
}

static std::unique_ptr<codesh::ast::collection::range_ast_node> parse_range(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        std::unique_ptr<codesh::ast::var_reference::value_ast_node> eval_ast_node)
{
    auto to_val = codesh::parser::value::parse_value(tokens);

    std::unique_ptr<codesh::ast::var_reference::value_ast_node> skip_val = nullptr;
    if (codesh::parser::util::consuming_check(tokens, codesh::token_group::KEYWORD_SKIP))
    {
        skip_val = codesh::parser::value::parse_value(tokens);
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
