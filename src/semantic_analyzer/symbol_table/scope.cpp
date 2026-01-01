#include "scope.h"

#include "symbol.h"

codesh::semantic_analyzer::symbols_collection::symbols_collection(std::vector<symbol_type> allowed_symbol_types) :
    allowed_symbol_types(std::move(allowed_symbol_types))
{
}

codesh::semantic_analyzer::symbols_collection::~symbols_collection() = default;

codesh::semantic_analyzer::named_symbol_map::named_symbol_map(std::vector<symbol_type> allowed_symbol_types) :
    symbols_collection(std::move(allowed_symbol_types))
{
}

std::optional<std::reference_wrapper<codesh::semantic_analyzer::symbol>> codesh::semantic_analyzer::
    named_symbol_map::resolve_local(const std::string &name) const
{
    const auto result = symbols.find(name);

    if (result == symbols.end())
        return std::nullopt;

    return *result->second;
}

std::unique_ptr<codesh::semantic_analyzer::symbol> codesh::semantic_analyzer::named_symbol_map::resolve_and_move(
    const std::string &name)
{
    const auto result = symbols.find(name);

    if (result == symbols.end())
        return nullptr;

    std::unique_ptr<symbol> symbol = std::move(result->second);
    // This symbol is about to be moved out of this table -
    // so remove its duplicate entry from here.
    symbols.erase(result);

    return symbol;
}

bool codesh::semantic_analyzer::named_symbol_map::is_empty() const
{
    return symbols.empty();
}

const std::unordered_map<std::string, std::unique_ptr<codesh::semantic_analyzer::symbol>> &codesh::semantic_analyzer::
    named_symbol_map::internals() const
{
    return symbols;
}

void codesh::semantic_analyzer::named_symbol_map::remove_symbol(const std::string &name)
{
    symbols.erase(name);
}

codesh::semantic_analyzer::symbol_list::symbol_list(std::vector<symbol_type> allowed_symbol_types)
    : symbols_collection(std::move(allowed_symbol_types))
{
}

std::optional<std::reference_wrapper<codesh::semantic_analyzer::symbol>> codesh::semantic_analyzer::symbol_list::
    resolve(const size_t index) const
{
    return *symbols.at(index);
}

std::unique_ptr<codesh::semantic_analyzer::symbol> codesh::semantic_analyzer::symbol_list::resolve_and_move(
    const size_t index)
{
    return std::move(symbols.at(index));
}

bool codesh::semantic_analyzer::symbol_list::is_empty() const
{
    return symbols.empty();
}

void codesh::semantic_analyzer::symbol_list::remove_symbol(const size_t index)
{
    auto it = symbols.begin();
    std::advance(it, index);
    symbols.erase(it);
}
