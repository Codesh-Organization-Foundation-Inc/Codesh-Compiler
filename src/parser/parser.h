#pragma once

#include "token/token.h"
#include "parser/ast/compilation_unit_ast_node.h"

#include <filesystem>
#include <memory>
#include <queue>

namespace codesh::parser
{
[[nodiscard]] std::unique_ptr<ast::compilation_unit_ast_node> parse(std::queue<std::unique_ptr<token>> &tokens,
        const std::filesystem::path &source_path);
}
