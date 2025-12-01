#pragma once

namespace codesh::semantic_analyzer
{
class type_symbol;
}


namespace codesh::semantic_analyzer::method_declaration
{
void resolve_aliases(type_symbol &type);
}
