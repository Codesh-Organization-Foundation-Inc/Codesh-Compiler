#include "biblical_numbers_parser.h"

#include "parser/util.h"

#include <set>
#include <unordered_map>

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

// To save up on calculations, these are the only relevant powers of 10 (as defined by the available keywords)
static const std::set POWERS_OF_10 = {
    10, 100, 1000, 10000
};

static std::optional<biblical_number> parse_biblical_number(codesh::token_group token_group);


std::unique_ptr<codesh::ast::var_reference::value_ast_node> codesh::parser::value::parse_biblical_value(
    std::queue<std::unique_ptr<token>> &tokens)
{
    // The first number must not be an addition
    bool allow_addition = false;

    bool encountered_period = false;
    bool previous_was_period = false;

    // Value refers to the literal current number
    int previous_value = std::numeric_limits<int>::min();
    // Distro refers to a group of numbers forming one number distribution
    // These are separated by addition and united via multiplication
    // יעני פילוג לא יודע איך לרשום
    int previous_distro = std::numeric_limits<int>::min();

    int result = 0;
    int current_number_distro = 0;

    while (!tokens.empty())
    {
        const std::unique_ptr<token> token = std::move(tokens.front());
        tokens.pop();

        const auto number = parse_biblical_number(token->get_group());
        if (!number.has_value())
        {
            if (previous_was_period)
            {
                //TODO: Incomplete number blasphemy
            }

            result += current_number_distro;
            break;
        }

        if (number->is_period)
        {
            if (encountered_period)
            {
                //TODO: Can't have 2 periods blasphemy
            }

            encountered_period = true;
            previous_was_period = true;

            continue;
        }

        // The first number must not be an addition
        if (!allow_addition)
        {
            if (number->is_addition)
            {
                //TODO: Throw invalid addition blasphemy
            }

            allow_addition = true;
        }


        // Handle value insertion
        const int num_value = **number;
        if (number->is_addition)
        {
            // The order of the distros must be in ascending order
            if (current_number_distro <= previous_distro)
            {
                //TODO: Throw invalid order blasphemy (type distro (mikbatz))
            }

            // Add the distro to the result
            result += current_number_distro;

            // Start a new distro
            previous_distro = current_number_distro;
            current_number_distro = num_value;

            // Reset the ascending numbers requirement
            previous_value = std::numeric_limits<int>::min();
        }
        else
        {
            // The order of the numbers must be in ascending order
            if (num_value <= previous_value)
            {
                //TODO: Throw invalid order blasphemy (type number)
            }

            // Only powers of 10 should be used as multiplications
            if (!POWERS_OF_10.contains(num_value))
            {
                //TODO: Throw blasphemy
            }

            current_number_distro *= num_value;
        }

        previous_was_period = false;
        previous_value = num_value;
    }

    return nullptr;
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

static std::optional<biblical_number> parse_biblical_number(const codesh::token_group token_group)
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
