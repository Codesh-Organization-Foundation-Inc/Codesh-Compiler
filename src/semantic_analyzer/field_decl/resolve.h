#pragma once

namespace codesh::semantic_analyzer
{
struct semantic_context;
class type_symbol;
}
namespace codesh::ast::type_decl
{
class field_declaration_ast_node;
}


namespace codesh::semantic_analyzer::field_declaration
{
void resolve(const semantic_context &context, const type_symbol &type,
        const ast::type_decl::field_declaration_ast_node &field_decl);
}
