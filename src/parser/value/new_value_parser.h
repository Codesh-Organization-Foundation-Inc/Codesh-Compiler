#pragma once

#include "parser/ast/method/operation/new_ast_node.h"

#include <memory>
#include <queue>

namespace codesh
{
class token;
}

namespace codesh::parser::value
{
[[nodiscard]] std::unique_ptr<ast::op::new_ast_node> parse_new_operator(
        std::queue<std::unique_ptr<token>> &tokens);
}
