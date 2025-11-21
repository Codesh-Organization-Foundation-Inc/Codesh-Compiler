#pragma once
#include "../../parser/ast/compilation_unit_ast_node.h"
#include "../../parser/ast/type_declaration/class_declaration_ast_node.h"


namespace codesh::semantic_analyzer
{
void check_methods(const ast::compilation_unit_ast_node &root);
}
