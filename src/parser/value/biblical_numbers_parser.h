#pragma once

#include "parser/ast/var_reference/value_ast_node.h"
#include "token/token.h"

#include <memory>
#include <queue>

namespace codesh::parser::value
{
struct biblical_number;


class biblical_numbers_parser
{
    std::queue<std::unique_ptr<token>> &tokens;

    static std::optional<biblical_number> parse_biblical_number(token_group token_group);

public:
    explicit biblical_numbers_parser(std::queue<std::unique_ptr<token>> &tokens);

    [[nodiscard]] std::unique_ptr<ast::var_reference::value_ast_node> parse() const;
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
