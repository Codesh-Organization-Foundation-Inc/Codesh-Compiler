#pragma once

#include "ast/var_reference/value_ast_node.h"

#include <memory>
#include <queue>

namespace codesh
{
class token;
}

namespace codesh::parser
{
std::unique_ptr<ast::var_reference::value_ast_node> parse_value(std::queue<std::unique_ptr<token>> &tokens);
std::unique_ptr<ast::var_reference::value_ast_node> parse_boolean_value(std::queue<std::unique_ptr<token>> &tokens);
}
