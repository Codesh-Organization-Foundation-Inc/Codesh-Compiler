#pragma once
#include "ast/compilation_unit_ast_node.h"

#include <memory>
#include <queue>

namespace codesh::parser
{

std::unique_ptr<ast::compilation_unit_ast_node> parse_compilation_unit(std::queue<std::unique_ptr<token>> &tokens,
    const std::string &source_stem);

}