#pragma once
#include <string>

namespace codesh::semantic_analyzer
{

enum class SymbolKind
{
    VARIABLE,
    TYPE,
    METHOD,
    FIELD,
    PARAMETER
};

struct Symbol
{
    std::string name;
    SymbolKind kind;
    void* ast_node;

    Symbol(std::string name, SymbolKind kind, void* node);
};

}
