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
inline constexpr std::string ALIAS_OBJECT = "לאבובו";

void collect_builtins(const symbol_table &table);
}
