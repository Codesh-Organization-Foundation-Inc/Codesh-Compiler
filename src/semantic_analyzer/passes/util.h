#pragma once
#include <string>

namespace codesh::ast
{
class compilation_unit_ast_node;
}

namespace codesh::semantic_analyzer::util
{
bool type_exists(const ast::compilation_unit_ast_node &root, const std::string &name);
}
