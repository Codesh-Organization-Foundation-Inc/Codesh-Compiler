#pragma once
#include "../../parser/ast/compilation_unit_ast_node.h"

namespace codesh::semantic_analyzer
{

static bool type_exists(const ast::compilation_unit_ast_node &root, const std::string &name);
void check_methods(const ast::compilation_unit_ast_node &root);
}
