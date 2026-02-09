#pragma once

#include "parser/ast/var_reference/value_ast_node.h"
#include "token/token.h"

#include <memory>
#include <queue>
#include <set>

namespace codesh::parser::value
{
struct biblical_number
{
    std::optional<int> value;

    bool is_addition;
    bool is_period;
};


class biblical_numbers_parser
{
    std::queue<std::unique_ptr<token>> &tokens;

    std::optional<biblical_number> current_number;
    std::optional<biblical_number> next_number;

    bool contains_period;

    // יעני פילוג
    int previous_distro;
    int current_distro;

    int result;


    // Simulate an automaton.
    // See: https://excalidraw.com/#json=bSaEqnTKpd8ElGsLu32lH,G9BrmusT0Oe0phj-kjjIRw
    enum class parsing_state
    {
        START,

        HANDLE_ADDITION,
        HANDLE_MULTIPLICATION,
        HANDLE_PERIOD
    };

    static const std::set<parsing_state> ACCEPTING_STATES;

    // Each method returns the new parsing state
    [[nodiscard]] parsing_state start() const;

    [[nodiscard]] parsing_state handle_addition();
    [[nodiscard]] parsing_state handle_multiplication();
    [[nodiscard]] parsing_state handle_next_number() const;

    [[nodiscard]] parsing_state handle_period();

    [[nodiscard]] static parsing_state handle_invalid_addition();
    [[nodiscard]] static parsing_state handle_invalid_period();
    [[nodiscard]] static parsing_state handle_invalid_mid_period();

    /**
     * Collects all biblical number tokens, parses them, and puts them in order at accumulated_numbers.
     *
     * Throws parsing errors when necessary along the way.
     */
    void collect_numbers();

    [[nodiscard]] std::optional<biblical_number> parse_biblical_number() const;

public:
    explicit biblical_numbers_parser(std::queue<std::unique_ptr<token>> &tokens);

    [[nodiscard]] std::unique_ptr<ast::var_reference::value_ast_node> parse();
};


constexpr int operator*(const biblical_number num)
{
    return num.value.value();
}

}
