#pragma once

#include "ast/impl/ast_node.h"
#include "../token/token.h"

#include <memory>
#include <queue>

namespace codesh::parser
{
[[nodiscard]] std::unique_ptr<ast::impl::ast_node> parse(std::queue<std::unique_ptr<token>> &tokens);
}
