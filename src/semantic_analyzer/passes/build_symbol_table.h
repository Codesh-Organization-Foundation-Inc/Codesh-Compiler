#pragma once
#include "../../parser/ast/compilation_unit_ast_node.h"

namespace codesh::semantic
{
void build_symbol_table(ast::compilation_unit_ast_node& root);
}
