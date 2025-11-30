#pragma once

namespace codesh::ast
{
class compilation_unit_ast_node;
}


namespace codesh::semantic_analyzer::method_declaration
{
void resolve_methods(const ast::compilation_unit_ast_node &root);
}