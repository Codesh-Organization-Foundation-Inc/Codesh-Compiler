#pragma once

namespace codesh::semantic_analyzer
{
class country_symbol;
}
namespace codesh::semantic_analyzer
{
struct semantic_context;
}
namespace codesh::ast
{
class compilation_unit_ast_node;
}

namespace codesh::semantic_analyzer::type_declaration
{
void resolve(const semantic_context &context, const country_symbol &country);
}
