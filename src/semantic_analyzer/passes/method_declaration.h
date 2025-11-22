#pragma once

#include "../../parser/ast/type_declaration/class_declaration_ast_node.h"


namespace codesh::semantic_analyzer
{
class type_symbol;
void collect_methods(const ast::type_decl::class_declaration_ast_node &class_decl, type_symbol &containing_type);

void check_methods(ast::compilation_unit_ast_node &root);
}
