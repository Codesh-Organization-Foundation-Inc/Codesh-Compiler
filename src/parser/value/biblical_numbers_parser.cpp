#include "biblical_numbers_parser.h"

#include "parser/ast/type/primitive_type_ast_node.h"
#include "parser/ast/var_reference/evaluable_ast_node.h"
#include "parser/util.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "lexer/trie/keywords.h"

#include <unordered_map>

// To save up on calculations, these are the only relevant powers of 10 (as defined by the available keywords)
static const std::unordered_set POWERS_OF_10 = {
    10, 100, 1000, 10000
};

const std::unordered_set<codesh::parser::value::biblical_numbers_parser::parsing_state> codesh::parser::value::
    biblical_numbers_parser::ACCEPTING_STATES =
{
    parsing_state::START,
    parsing_state::HANDLE_ADDITION,
    parsing_state::HANDLE_MULTIPLICATION
};

codesh::parser::value::biblical_numbers_parser::biblical_numbers_parser(std::queue<std::unique_ptr<token>> &tokens) :
    tokens(tokens),
    contains_period(false),
    previous_distro(std::numeric_limits<int>::min()),
    current_distro(0),
    result(0),
    integer_result(0)
{
}

std::unique_ptr<codesh::ast::var_reference::value_ast_node> codesh::parser::value::biblical_numbers_parser::parse()
{
    const auto pos = tokens.front()->get_code_position();
    collect_numbers();

    if (contains_period)
    {
        // Convert the fractional integer to a decimal fraction (e.g. 23 -> 0.23),
        // then add it to the result.
        double divisor = 1.0;
        while (divisor <= result)
            divisor *= 10;

        const double value = integer_result + result / divisor;

        if (util::consuming_check(tokens, token_group::KEYWORD_FLOAT))
        {
            return std::make_unique<ast::var_reference::evaluable_ast_node<float>>(
                pos,
                std::make_unique<ast::type::primitive_type_ast_node>(pos, definition::primitive_type::FLOAT),
                static_cast<float>(value)
            );
        }

        return std::make_unique<ast::var_reference::evaluable_ast_node<double>>(
            pos,
            std::make_unique<ast::type::primitive_type_ast_node>(pos, definition::primitive_type::DOUBLE),
            value
        );
    }

    return std::make_unique<ast::var_reference::evaluable_ast_node<int>>(
        pos,
        std::make_unique<ast::type::primitive_type_ast_node>(pos, definition::primitive_type::INTEGER),
        result
    );
}

void codesh::parser::value::biblical_numbers_parser::collect_numbers()
{
    auto parse_state = parsing_state::START;
    while (!tokens.empty())
    {
        current_number = std::move(next_number);
        next_number = parse_biblical_number();

        // Null is valid to be had only for the start (current) and end (next) states.
        // If both are null, it means we don't read a number anymore.
        if (!current_number.has_value() && !next_number.has_value())
            break;

        // Perform state actions
        switch (parse_state)
        {
        case parsing_state::HANDLE_ADDITION:
            handle_addition();
            break;
        case parsing_state::HANDLE_MULTIPLICATION:
            handle_multiplication();
            break;
        case parsing_state::HANDLE_PERIOD:
            handle_period();
            break;

        default: break;
        }

        // Transition
        // Just end execution if the next one is empty
        if (!next_number.has_value())
            break;

        switch (parse_state)
        {
        case parsing_state::START:
            parse_state = start_transition();
            break;
        case parsing_state::HANDLE_ADDITION:
        case parsing_state::HANDLE_MULTIPLICATION:
            parse_state = next_number_transition();
            break;
        case parsing_state::HANDLE_PERIOD:
            parse_state = handle_period_transition();
            break;
        }
    }

    if (!ACCEPTING_STATES.contains(parse_state))
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(
            fmt::format(
                blasphemy::details::UNEXPECTED_END_OF_NUMBER,
                lexer::trie::TOKEN_TO_NAME_MAP.at(current_number->producing_token->get_group())
            ),
            blasphemy::blasphemy_type::SYNTAX,
            current_number->producing_token->get_code_position()
        );
    }

    result += current_distro;
}


void codesh::parser::value::biblical_numbers_parser::handle_addition()
{
    result += current_distro;

    // Distro order should be ascending
    if (previous_distro >= current_distro)
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(
            fmt::format(
                blasphemy::details::INVALID_NUMBER_FORMAT_ASCENDING,
                lexer::trie::TOKEN_TO_NAME_MAP.at(current_number->producing_token->get_group())
            ),
            blasphemy::blasphemy_type::SYNTAX,
            current_number->producing_token->get_code_position()
        );
    }

    previous_distro = current_distro;
    current_distro = **current_number;
}

void codesh::parser::value::biblical_numbers_parser::handle_multiplication()
{
    current_distro *= **current_number;

    // Can only multiply by powers of 10
    if (!POWERS_OF_10.contains(**current_number))
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(
            fmt::format(
                blasphemy::details::INVALID_MULTIPLICATION_FACTOR,
                lexer::trie::TOKEN_TO_NAME_MAP.at(current_number->producing_token->get_group())
            ),
            blasphemy::blasphemy_type::SYNTAX,
            current_number->producing_token->get_code_position()
        );
    }

    // Number order should be ascending within the same distro
    if (next_number.has_value() && !next_number->is_addition)
    {
        if (**current_number >= **next_number)
        {
            blasphemy::get_blasphemy_collector().add_blasphemy(
                fmt::format(
                    blasphemy::details::INVALID_NUMBER_FORMAT_DESCENDING,
                    lexer::trie::TOKEN_TO_NAME_MAP.at(next_number->producing_token->get_group()),
                    lexer::trie::TOKEN_TO_NAME_MAP.at(current_number->producing_token->get_group())
                ),
                blasphemy::blasphemy_type::SYNTAX,
                next_number->producing_token->get_code_position()
            );
        }
    }
}

void codesh::parser::value::biblical_numbers_parser::handle_period()
{
    contains_period = true;

    // Move the results to integer_result
    integer_result = result + current_distro;
    // Now results will hold the decimal part
    result = 0;

    // Reset distro
    current_distro = 0;
    previous_distro = std::numeric_limits<int>::min();
}

codesh::parser::value::biblical_numbers_parser::parsing_state codesh::parser::value::biblical_numbers_parser::
    start_transition() const
{
    if (next_number->is_addition)
        return handle_invalid_addition();
    if (next_number->is_period)
        return handle_invalid_period();

    return parsing_state::HANDLE_ADDITION;
}

codesh::parser::value::biblical_numbers_parser::parsing_state codesh::parser::value::biblical_numbers_parser::
    next_number_transition() const
{
    if (next_number->is_addition)
        return parsing_state::HANDLE_ADDITION;

    if (next_number->is_period)
    {
        if (contains_period)
            return handle_invalid_mid_period();

        return parsing_state::HANDLE_PERIOD;
    }

    return parsing_state::HANDLE_MULTIPLICATION;
}

codesh::parser::value::biblical_numbers_parser::parsing_state codesh::parser::value::biblical_numbers_parser::
    handle_period_transition() const
{
    if (next_number->is_addition)
        return handle_invalid_addition();
    if (next_number->is_period)
        return handle_invalid_period();

    return parsing_state::START;
}

codesh::parser::value::biblical_numbers_parser::parsing_state codesh::parser::value::biblical_numbers_parser::
    handle_invalid_addition() const
{
    blasphemy::get_blasphemy_collector().add_blasphemy(
        fmt::format(
            blasphemy::details::UNEXPECTED_ADDITION_OPERATOR,
            lexer::trie::TOKEN_TO_NAME_MAP.at(next_number->producing_token->get_group())
        ),
        blasphemy::blasphemy_type::SYNTAX,
        next_number->producing_token->get_code_position()
    );
    return parsing_state::HANDLE_ADDITION;
}

codesh::parser::value::biblical_numbers_parser::parsing_state codesh::parser::value::biblical_numbers_parser::
    handle_invalid_period() const
{
    blasphemy::get_blasphemy_collector().add_blasphemy(
        blasphemy::details::UNEXPECTED_PERIOD_OPERATOR,
        blasphemy::blasphemy_type::SYNTAX,
        next_number->producing_token->get_code_position()
    );
    return parsing_state::START;
}

codesh::parser::value::biblical_numbers_parser::parsing_state codesh::parser::value::biblical_numbers_parser::
    handle_invalid_mid_period() const
{
    blasphemy::get_blasphemy_collector().add_blasphemy(
        blasphemy::details::MULTIPLE_PERIOD_OPERATORS,
        blasphemy::blasphemy_type::SYNTAX,
        next_number->producing_token->get_code_position()
    );
    return parsing_state::HANDLE_ADDITION;
}

static const std::unordered_map<codesh::token_group, int> BIBLICAL_NUMBER_TOKEN_TO_VALUE = {
    {codesh::token_group::KEYWORD_BIBLICAL_ZERO, 0},
    {codesh::token_group::KEYWORD_BIBLICAL_ZERO_ADDED, 0},
    {codesh::token_group::KEYWORD_BIBLICAL_ONE, 1},
    {codesh::token_group::KEYWORD_BIBLICAL_ONE_ADDED, 1},
    {codesh::token_group::KEYWORD_BIBLICAL_TWO, 2},
    {codesh::token_group::KEYWORD_BIBLICAL_TWO_ADDED, 2},
    {codesh::token_group::KEYWORD_BIBLICAL_THREE, 3},
    {codesh::token_group::KEYWORD_BIBLICAL_THREE_ADDED, 3},
    {codesh::token_group::KEYWORD_BIBLICAL_FOUR, 4},
    {codesh::token_group::KEYWORD_BIBLICAL_FOUR_ADDED, 4},
    {codesh::token_group::KEYWORD_BIBLICAL_FIVE, 5},
    {codesh::token_group::KEYWORD_BIBLICAL_FIVE_ADDED, 5},
    {codesh::token_group::KEYWORD_BIBLICAL_SIX, 6},
    {codesh::token_group::KEYWORD_BIBLICAL_SIX_ADDED, 6},
    {codesh::token_group::KEYWORD_BIBLICAL_SEVEN, 7},
    {codesh::token_group::KEYWORD_BIBLICAL_SEVEN_ADDED, 7},
    {codesh::token_group::KEYWORD_BIBLICAL_EIGHT, 8},
    {codesh::token_group::KEYWORD_BIBLICAL_EIGHT_ADDED, 8},
    {codesh::token_group::KEYWORD_BIBLICAL_NINE, 9},
    {codesh::token_group::KEYWORD_BIBLICAL_NINE_ADDED, 9},
    {codesh::token_group::KEYWORD_BIBLICAL_TEN, 10},
    {codesh::token_group::KEYWORD_BIBLICAL_TEN_ADDED, 10},
    {codesh::token_group::KEYWORD_BIBLICAL_TWELVE, 12},
    {codesh::token_group::KEYWORD_BIBLICAL_TWELVE_ADDED, 12},
    {codesh::token_group::KEYWORD_BIBLICAL_TWENTY, 20},
    {codesh::token_group::KEYWORD_BIBLICAL_TWENTY_ADDED, 20},
    {codesh::token_group::KEYWORD_BIBLICAL_THIRTY, 30},
    {codesh::token_group::KEYWORD_BIBLICAL_THIRTY_ADDED, 30},
    {codesh::token_group::KEYWORD_BIBLICAL_FORTY, 40},
    {codesh::token_group::KEYWORD_BIBLICAL_FORTY_ADDED, 40},
    {codesh::token_group::KEYWORD_BIBLICAL_FIFTY, 50},
    {codesh::token_group::KEYWORD_BIBLICAL_FIFTY_ADDED, 50},
    {codesh::token_group::KEYWORD_BIBLICAL_SIXTY, 60},
    {codesh::token_group::KEYWORD_BIBLICAL_SIXTY_ADDED, 60},
    {codesh::token_group::KEYWORD_BIBLICAL_SEVENTY, 70},
    {codesh::token_group::KEYWORD_BIBLICAL_SEVENTY_ADDED, 70},
    {codesh::token_group::KEYWORD_BIBLICAL_EIGHTY, 80},
    {codesh::token_group::KEYWORD_BIBLICAL_EIGHTY_ADDED, 80},
    {codesh::token_group::KEYWORD_BIBLICAL_NINETY, 90},
    {codesh::token_group::KEYWORD_BIBLICAL_NINETY_ADDED, 90},
    {codesh::token_group::KEYWORD_BIBLICAL_HUNDRED, 100},
    {codesh::token_group::KEYWORD_BIBLICAL_HUNDRED_ADDED, 100},
    {codesh::token_group::KEYWORD_BIBLICAL_THOUSAND, 1000},
    {codesh::token_group::KEYWORD_BIBLICAL_THOUSAND_ADDED, 1000},
    {codesh::token_group::KEYWORD_BIBLICAL_TEN_THOUSAND, 10000},
    {codesh::token_group::KEYWORD_BIBLICAL_TEN_THOUSAND_ADDED, 10000},
};

std::optional<codesh::parser::value::biblical_number> codesh::parser::value::biblical_numbers_parser::
    parse_biblical_number() const
{
    const auto token_group = tokens.front()->get_group();

    // The enums are contiguous from KEYWORD_BIBLICAL_DECIMAL_SEPARATOR to KEYWORD_BIBLICAL_TEN_THOUSAND_ADDED;
    // Anything else is invalid.
    if (token_group < token_group::KEYWORD_BIBLICAL_DECIMAL_SEPARATOR
        || token_group > token_group::KEYWORD_BIBLICAL_TEN_THOUSAND_ADDED)
    {
        return std::nullopt;
    }

    auto token = util::consume_token(tokens);

    if (token_group == token_group::KEYWORD_BIBLICAL_DECIMAL_SEPARATOR)
    {
        return biblical_number {
            std::nullopt,
            false,
            true,
            std::move(token)
        };
    }

    return biblical_number {
        BIBLICAL_NUMBER_TOKEN_TO_VALUE.at(token_group),
        // All addition tokens are contiguous from KEYWORD_BIBLICAL_ZERO_ADDED to KEYWORD_BIBLICAL_TEN_THOUSAND_ADDED
        token_group::KEYWORD_BIBLICAL_ZERO_ADDED <= token_group
            && token_group <= token_group::KEYWORD_BIBLICAL_TEN_THOUSAND_ADDED,
        false,
        std::move(token)
    };
}
