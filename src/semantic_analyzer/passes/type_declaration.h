#pragma once

#include "../../parser/ast/compilation_unit_ast_node.h"

namespace codesh::semantic_analyzer::type_declaration
{
void collect_types(const ast::compilation_unit_ast_node &root_node, country_symbol &containing_country);
}
