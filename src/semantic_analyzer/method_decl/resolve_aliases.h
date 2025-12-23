#pragma once

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
void resolve_aliases(const semantic_context &context, type_symbol &type);
}
