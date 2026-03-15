#pragma once

namespace codesh::semantic_analyzer
{
struct semantic_context;
class country_symbol;
}


namespace codesh::semantic_analyzer::field_declaration
{
void collect(const semantic_context &context, country_symbol &country);
}
