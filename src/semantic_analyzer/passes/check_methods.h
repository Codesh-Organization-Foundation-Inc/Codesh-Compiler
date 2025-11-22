#pragma once

#include "../../parser/ast/type_declaration/class_declaration_ast_node.h"


namespace codesh::semantic_analyzer
{
class symbol_table;
void check_methods(ast::compilation_unit_ast_node &root);
}
