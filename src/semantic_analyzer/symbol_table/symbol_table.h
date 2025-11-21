#pragma once

#include "symbol.h"

namespace codesh::semantic_analyzer
{

class symbol_table
{
    std::unordered_map<std::string, package_symbol> global_scope;
};

}
