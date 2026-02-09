#pragma once

#include "parser/ast/var_reference/value_ast_node.h"

#include <memory>
#include <queue>

namespace codesh
{
class token;
}

namespace codesh::parser::value
{
std::unique_ptr<ast::var_reference::value_ast_node> parse_primitive_value(std::queue<std::unique_ptr<token>> &tokens);
}
