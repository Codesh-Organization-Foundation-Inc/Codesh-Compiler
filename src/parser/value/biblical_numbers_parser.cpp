#include "biblical_numbers_parser.h"

#include <unordered_map>

struct biblical_number;

static std::optional<biblical_number> parse_biblical_number_token(codesh::token_group token_group);


std::unique_ptr<codesh::ast::var_reference::value_ast_node> codesh::parser::value::parse_biblical_value(
    std::queue<std::unique_ptr<token>> &tokens)
{
    return nullptr;
}


struct biblical_number
{
    std::optional<int> value;

    bool is_addition;
    bool is_period;
};

constexpr int operator*(const biblical_number num)
{
    return num.value.value();
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

static std::optional<biblical_number> parse_biblical_number_token(const codesh::token_group token_group)
{
    // The enums are contiguous from KEYWORD_BIBLICAL_DECIMAL_SEPARATOR to KEYWORD_BIBLICAL_TEN_THOUSAND_ADDED;
    // Anything else is invalid.
    if (token_group < codesh::token_group::KEYWORD_BIBLICAL_DECIMAL_SEPARATOR
        || token_group > codesh::token_group::KEYWORD_BIBLICAL_TEN_THOUSAND_ADDED)
    {
        return std::nullopt;
    }

    if (token_group == codesh::token_group::KEYWORD_BIBLICAL_DECIMAL_SEPARATOR)
    {
        return biblical_number{std::nullopt, false, true};
    }

    return biblical_number {
        BIBLICAL_NUMBER_TOKEN_TO_VALUE.at(token_group),
        // All addition tokens are contiguous from KEYWORD_BIBLICAL_ZERO_ADDED to KEYWORD_BIBLICAL_TEN_THOUSAND_ADDED
        codesh::token_group::KEYWORD_BIBLICAL_ZERO_ADDED <= token_group
            && token_group <= codesh::token_group::KEYWORD_BIBLICAL_TEN_THOUSAND_ADDED
    };
}
