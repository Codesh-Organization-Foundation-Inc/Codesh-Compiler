#pragma once

#include <string>

namespace codesh::semantic_analyzer
{
class symbol_table;
}

namespace codesh::semantic_analyzer::builtins
{
inline constexpr std::string CLASS_RANGE = "טווח";

inline constexpr std::string ALIAS_STRING = "כתובים";
inline constexpr std::string ALIAS_LABUBU = "לאבובו";

void add_builtins(const symbol_table &table);
}
