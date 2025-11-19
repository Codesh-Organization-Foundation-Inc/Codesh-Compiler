#pragma once
#include "../../parser/ast/compilation_unit_ast_node.h"

namespace codesh::semantic
{
void check_types(ast::compilation_unit_ast_node& root);
}
