#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace codesh::semantic_analyzer
{
class symbol;

using named_scope_map = std::unordered_map<std::string, std::unique_ptr<symbol>>;
}
