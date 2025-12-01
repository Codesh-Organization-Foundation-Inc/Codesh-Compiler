#pragma once
#include "../../parser/ast/compilation_unit_ast_node.h"
#include "../../semantic_analyzer/errors/errors.h"

namespace codesh::semantic_analyzer
{
void check_methods(const ast::compilation_unit_ast_node &root, error_collector &error_storage);
}
