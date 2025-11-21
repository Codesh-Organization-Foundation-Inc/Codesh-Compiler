#pragma once

#include "../parser/ast/compilation_unit_ast_node.h"

namespace codesh::semantic_analyzer
{
class symbol_table;
}
namespace codesh::semantic_analyzer
{
void run(ast::compilation_unit_ast_node &ast_root, symbol_table &symbol_table);
}
