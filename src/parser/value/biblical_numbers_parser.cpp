#include "biblical_numbers_parser.h"

#include "parser/util.h"

#include <unordered_map>

// To save up on calculations, these are the only relevant powers of 10 (as defined by the available keywords)
static const std::set POWERS_OF_10 = {
    10, 100, 1000, 10000
};

const std::set<codesh::parser::value::biblical_numbers_parser::parsing_state> codesh::parser::value::
    biblical_numbers_parser::ACCEPTING_STATES =
{
    parsing_state::START,
    parsing_state::HANDLE_ADDITION,
    parsing_state::HANDLE_MULTIPLICATION,
    parsing_state::END
};

const std::set<codesh::parser::value::biblical_numbers_parser::parsing_state> codesh::parser::value::
    biblical_numbers_parser::ERROR_STATES =
{
    parsing_state::HANDLE_INVALID_ADDITION,
    parsing_state::HANDLE_INVALID_PERIOD,
    parsing_state::HANDLE_INVALID_MID_PERIOD,
};


codesh::parser::value::biblical_numbers_parser::biblical_numbers_parser(std::queue<std::unique_ptr<token>> &tokens) :
    tokens(tokens),
    contains_period(false)
{
}

std::unique_ptr<codesh::ast::var_reference::value_ast_node> codesh::parser::value::biblical_numbers_parser::parse()
{
    auto parse_state = parsing_state::START;
    while (parse_state != parsing_state::END)
    {
        auto number = parse_biblical_number();
        if (!number.has_value())
            break;

        switch (parse_state)
        {
        case parsing_state::START:
            parse_state = start(*number);
            break;
        case parsing_state::HANDLE_ADDITION:
            parse_state = handle_addition(*number);
            break;
        case parsing_state::HANDLE_MULTIPLICATION:
            parse_state = handle_multiplication(*number);
            break;
        case parsing_state::HANDLE_PERIOD:
            parse_state = handle_period(*number);
            break;
        case parsing_state::HANDLE_INVALID_ADDITION:
            parse_state = handle_invalid_addition(*number);
            break;
        case parsing_state::HANDLE_INVALID_PERIOD:
            parse_state = handle_invalid_period(*number);
            break;
        case parsing_state::HANDLE_INVALID_MID_PERIOD:
            parse_state = handle_invalid_mid_period(*number);
            break;

        default:
            throw std::runtime_error("Invalid state");
        }
    }

    if (!ACCEPTING_STATES.contains(parse_state))
    {
        // No need to throw an error twice
        if (!ERROR_STATES.contains(parse_state))
        {
            //TODO: Throw unexpected end blasphemy
        }
    }

    //TODO: Parse final number
    //TODO: Check ascending order
    return nullptr;
}


codesh::parser::value::biblical_numbers_parser::parsing_state codesh::parser::value::biblical_numbers_parser::start(
        const biblical_number &number)
{
    return parsing_state::END;
}

codesh::parser::value::biblical_numbers_parser::parsing_state codesh::parser::value::biblical_numbers_parser::
    handle_addition(const biblical_number &number)
{
    return parsing_state::END;
}

codesh::parser::value::biblical_numbers_parser::parsing_state codesh::parser::value::biblical_numbers_parser::
    handle_multiplication(const biblical_number &number)
{
    return parsing_state::END;
}

codesh::parser::value::biblical_numbers_parser::parsing_state codesh::parser::value::biblical_numbers_parser::
    handle_period(const biblical_number &number)
{
    return parsing_state::END;
}

codesh::parser::value::biblical_numbers_parser::parsing_state codesh::parser::value::biblical_numbers_parser::
    handle_invalid_addition(const biblical_number &number)
{
    return parsing_state::END;
}

codesh::parser::value::biblical_numbers_parser::parsing_state codesh::parser::value::biblical_numbers_parser::
    handle_invalid_period(const biblical_number &number)
{
    return parsing_state::END;
}

codesh::parser::value::biblical_numbers_parser::parsing_state codesh::parser::value::biblical_numbers_parser::
    handle_invalid_mid_period(const biblical_number &number)
{
    return parsing_state::END;
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

    if (token_group == token_group::KEYWORD_BIBLICAL_DECIMAL_SEPARATOR)
    {
        return biblical_number{std::nullopt, false, true};
    }

    return biblical_number {
        BIBLICAL_NUMBER_TOKEN_TO_VALUE.at(token_group),
        // All addition tokens are contiguous from KEYWORD_BIBLICAL_ZERO_ADDED to KEYWORD_BIBLICAL_TEN_THOUSAND_ADDED
        token_group::KEYWORD_BIBLICAL_ZERO_ADDED <= token_group
            && token_group <= token_group::KEYWORD_BIBLICAL_TEN_THOUSAND_ADDED
    };
}
