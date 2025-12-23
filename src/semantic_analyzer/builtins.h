#pragma once

namespace codesh::semantic_analyzer
{
class symbol_table;
}

namespace codesh::semantic_analyzer::builtins
{
void add_builtins(const symbol_table &table);
}
