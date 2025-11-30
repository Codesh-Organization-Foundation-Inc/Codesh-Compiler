#pragma once

namespace codesh::ast
{
class compilation_unit_ast_node;
}

namespace codesh::semantic_analyzer
{
void resolve_aliases(const ast::compilation_unit_ast_node &ast_root);
}