#include "symbol_table.h"

#include "../../blasphemy/blasphemy_collector.h"
#include "../../blasphemy/blasphemy_consumer.h"
#include "../semantic_context.h"

[[nodiscard]] static std::optional<std::reference_wrapper<codesh::semantic_analyzer::symbol>>
    resolve_method_from_scope_container(
        const codesh::semantic_analyzer::i_scope_containing_symbol &scope_container,
        std::vector<std::string>::const_iterator fqcn_start,
        std::vector<std::string>::const_iterator fqcn_end);


const std::vector<codesh::semantic_analyzer::symbol_type> &codesh::semantic_analyzer::symbol_table::
    allowed_symbol_types() const
{
    return ALLOWED_SYMBOL_TYPES;
}

codesh::semantic_analyzer::symbol_table::symbol_table(const ast::compilation_unit_ast_node &root_node)
{
    // Add the global scope
    add_symbol("", std::make_unique<country_symbol>(""));
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


std::optional<std::reference_wrapper<codesh::semantic_analyzer::symbol>> codesh::semantic_analyzer::symbol_table::
    resolve_from_imports(const semantic_context &context,
        const std::vector<std::string>::const_iterator fqcn_start,
        const std::vector<std::string>::const_iterator fqcn_end)
{
    for (const auto &country : context.lookup_countries)
    {
         const auto result = resolve_method_from_scope_container(
             country,
             fqcn_start,
             fqcn_end
         );

        if (result.has_value())
            return result.value();
    }

    return std::nullopt;
}

static std::optional<std::reference_wrapper<codesh::semantic_analyzer::symbol>> resolve_method_from_scope_container(
        const codesh::semantic_analyzer::i_scope_containing_symbol &scope_container,
        const std::vector<std::string>::const_iterator fqcn_start,
        const std::vector<std::string>::const_iterator fqcn_end)
{
    auto it = fqcn_start;
    const auto end = fqcn_end;

    if (it == end)
        return std::nullopt;


    const auto symbol_raw = scope_container.resolve(*it);
    if (!symbol_raw.has_value())
        return std::nullopt;

    // Successfully consumed the FQCN part
    ++it;

    if (it == end)
    {
        // This must be the last symbol
        return symbol_raw;
    }

    if (const auto inner_scope_container =
        dynamic_cast<const codesh::semantic_analyzer::i_scope_containing_symbol *>(&symbol_raw->get()))
    {
        return resolve_method_from_scope_container(*inner_scope_container, it, end);
    }


    // There are still more FQCN parts, yet we can't go to them.
    // This means that it simply doesn't exist.
    return std::nullopt;
}

const std::vector<codesh::semantic_analyzer::symbol_type> codesh::semantic_analyzer::symbol_table::ALLOWED_SYMBOL_TYPES = {
    symbol_type::COUNTRY,
    // For the global scope
    symbol_type::TYPE
};
