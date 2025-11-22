#pragma once

#include "../parser/ast/compilation_unit_ast_node.h"

namespace codesh::semantic_analyzer
{
class symbol_table;
}
namespace codesh::semantic_analyzer
{
void setup_ast(const ast::compilation_unit_ast_node &ast_root);
void analyze(const ast::compilation_unit_ast_node &ast_root);
}
