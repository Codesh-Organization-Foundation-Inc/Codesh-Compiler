#pragma once

#include "parser/ast/var_reference/value_ast_node.h"
#include "token/token.h"

#include <memory>
#include <queue>
#include <set>

namespace codesh::parser::value
{
struct biblical_number;

class biblical_numbers_parser
{
    std::queue<std::unique_ptr<token>> &tokens;
    std::vector<biblical_number> accumulated_numbers;

    bool contains_period;

    [[nodiscard]] std::optional<biblical_number> parse_biblical_number() const;

    // Simulate an automaton.
    // See: https://excalidraw.com/#json=bSaEqnTKpd8ElGsLu32lH,G9BrmusT0Oe0phj-kjjIRw
    enum class parsing_state
    {
        START,

        HANDLE_ADDITION,
        HANDLE_MULTIPLICATION,
        HANDLE_PERIOD,

        HANDLE_INVALID_ADDITION,
        HANDLE_INVALID_PERIOD,
        HANDLE_INVALID_MID_PERIOD,

        END
    };

    static const std::set<parsing_state> ACCEPTING_STATES;
    static const std::set<parsing_state> ERROR_STATES;

    // Each method returns the new parsing state
    parsing_state start(const biblical_number &number);

    parsing_state handle_addition(const biblical_number &number);
    parsing_state handle_multiplication(const biblical_number &number);
    parsing_state handle_period(const biblical_number &number);

    parsing_state handle_invalid_addition(const biblical_number &number);
    parsing_state handle_invalid_period(const biblical_number &number);
    parsing_state handle_invalid_mid_period(const biblical_number &number);

public:
    explicit biblical_numbers_parser(std::queue<std::unique_ptr<token>> &tokens);

    [[nodiscard]] std::unique_ptr<ast::var_reference::value_ast_node> parse();
};


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

}
