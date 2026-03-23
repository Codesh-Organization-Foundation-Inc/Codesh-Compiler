#pragma once

#include <string>

namespace codesh::semantic_analyzer
{
class country_symbol;
class symbol_table;
}

namespace codesh::semantic_analyzer::builtins
{
inline constexpr std::string CLASS_RANGE = "טווח";

inline constexpr std::string ALIAS_STRING = "כתובים";
inline constexpr std::string ALIAS_OBJECT = "לאבובו";
inline constexpr std::string ALIAS_EXCEPTION = "חטא";

void collect_builtins(country_symbol &country);
}
