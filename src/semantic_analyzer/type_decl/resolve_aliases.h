#pragma once

namespace codesh::semantic_analyzer
{
class country_symbol;
}
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


namespace codesh::semantic_analyzer::type_declaration
{
void resolve_aliases(const semantic_context &context, const country_symbol &country);
}
