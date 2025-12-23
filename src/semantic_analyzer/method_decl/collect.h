#pragma once

namespace codesh::ast::type_decl
{
class type_declaration_ast_node;
}
namespace codesh::semantic_analyzer
{
struct semantic_context;
}
namespace codesh::semantic_analyzer
{
class type_symbol;
}


namespace codesh::semantic_analyzer::method_declaration
{
void collect_methods(const semantic_context &context, const ast::type_decl::type_declaration_ast_node &type_decl,
        type_symbol &containing_type);
}
