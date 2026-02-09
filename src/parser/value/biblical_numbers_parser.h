#pragma once

#include "parser/ast/var_reference/value_ast_node.h"
#include "token/token.h"

#include <memory>
#include <queue>

namespace codesh::parser::value
{
std::unique_ptr<ast::var_reference::value_ast_node> parse_biblical_value(std::queue<std::unique_ptr<token>> &tokens);
}
