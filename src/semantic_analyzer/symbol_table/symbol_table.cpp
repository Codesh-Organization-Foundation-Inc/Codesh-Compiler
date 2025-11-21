#include "symbol_table.h"

std::vector<codesh::semantic_analyzer::symbol_type> codesh::semantic_analyzer::symbol_table::allowed_symbol_types()
    const
{
    return ALLOWED_SYMBOL_TYPES;
}

const codesh::semantic_analyzer::named_scope_map &codesh::semantic_analyzer::symbol_table::get_symbol_map() const
{
    return global_scope;
}

codesh::semantic_analyzer::named_scope_map &codesh::semantic_analyzer::symbol_table::get_symbol_map()
{
    return global_scope;
}

std::optional<std::reference_wrapper<codesh::semantic_analyzer::country_symbol>> codesh::semantic_analyzer::
    symbol_table::resolve_country(const std::string &name) const
{
    const auto result = resolve(name);

    if (!result)
        return std::nullopt;

    // Only packages allowed anyway
    return *static_cast<country_symbol *>(&result.value().get()); // NOLINT(*-pro-type-static-cast-downcast)
}

const std::vector<codesh::semantic_analyzer::symbol_type> codesh::semantic_analyzer::symbol_table::ALLOWED_SYMBOL_TYPES = {
    symbol_type::COUNTRY,
    // For the global scope
    symbol_type::TYPE
};
