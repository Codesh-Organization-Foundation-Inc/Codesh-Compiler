#pragma once

#include "../../parser/ast/compilation_unit_ast_node.h"

namespace codesh::semantic_analyzer
{
class symbol_table;
void add_type_declarations(ast::compilation_unit_ast_node &root_node);
}
