#pragma once

#include "../../parser/ast/compilation_unit_ast_node.h"

namespace codesh::semantic_analyzer
{
void check_type_declarations(ast::compilation_unit_ast_node &root, error_collector &error_storage);
}
