#pragma once

#include "../token/token.h"
#include "ast/compilation_unit_ast_node.h"
#include "ast/impl/ast_node.h"

#include <memory>
#include <queue>

namespace codesh::parser
{
[[nodiscard]] std::unique_ptr<ast::compilation_unit_ast_node> parse(std::queue<std::unique_ptr<token>> &tokens);
}
