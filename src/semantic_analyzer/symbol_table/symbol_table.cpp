#include "symbol_table.h"

#include "../../blasphemy/blasphemy_collector.h"
#include "../../blasphemy/blasphemy_consumer.h"
#include "../type_decl/collect.h"
#include "../semantic_context.h"

const std::vector<codesh::semantic_analyzer::symbol_type> &codesh::semantic_analyzer::symbol_table::
    allowed_symbol_types() const
{
    return ALLOWED_SYMBOL_TYPES;
}

codesh::semantic_analyzer::symbol_table::symbol_table(const ast::compilation_unit_ast_node &root_node)
{
    // Add global scope to symbol table
    add_symbol("", std::make_unique<country_symbol>());


    //TODO: Resolve all countries of origin
    const std::vector lookup_countries = {
        resolve_country("").value()
    };

    const semantic_context context = {lookup_countries, root_node, blasphemy::semantic_consumer};

    country_symbol &country = lookup_countries.back();

    //TODO: Iterate over each and every country, then collect types.
    type_declaration::collect_types(
        context,
        country
    );
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
