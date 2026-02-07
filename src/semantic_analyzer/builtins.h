#pragma once

#include <string>

namespace codesh::semantic_analyzer
{
class symbol_table;
}

namespace codesh::semantic_analyzer::builtins
{
inline const std::string CLASS_RANGE = "טווח";

void add_builtins(const symbol_table &table);
}
