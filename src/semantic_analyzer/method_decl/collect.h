#pragma once

namespace codesh::ast::type_decl
{
class class_declaration_ast_node;
}
namespace codesh::semantic_analyzer
{
class type_symbol;
}


namespace codesh::semantic_analyzer::method_declaration
{
void collect_methods(const ast::type_decl::class_declaration_ast_node &class_decl, type_symbol &containing_type);
}
