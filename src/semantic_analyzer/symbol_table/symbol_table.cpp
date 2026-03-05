#include "symbol_table.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/blasphemy_consumer.h"
#include "defenition/definitions.h"
#include "semantic_analyzer/semantic_context.h"

[[nodiscard]] static std::optional<std::reference_wrapper<codesh::semantic_analyzer::symbol>>
    resolve_method_from_scope_container(
        const codesh::semantic_analyzer::i_scope_containing_symbol &scope_container,
        std::vector<std::string>::const_iterator fqn_start,
        std::vector<std::string>::const_iterator fqn_end);

codesh::semantic_analyzer::symbol_table::symbol_table() :
    scope(ALLOWED_SYMBOL_TYPES)
{
    // Add the global scope
    scope.add_symbol("", std::make_unique<country_symbol>(""));
}

const codesh::semantic_analyzer::named_symbol_map &codesh::semantic_analyzer::symbol_table::get_scope() const
{
    return scope;
}

std::optional<std::reference_wrapper<codesh::semantic_analyzer::country_symbol>> codesh::semantic_analyzer::
    symbol_table::resolve_country(const std::string &name) const
{
    const auto result = scope.resolve_local(name);

    if (!result)
        return std::nullopt;

    // Only packages allowed anyway
    return *static_cast<country_symbol *>(&result.value().get()); // NOLINT(*-pro-type-static-cast-downcast)
}


std::optional<std::reference_wrapper<codesh::semantic_analyzer::symbol>> codesh::semantic_analyzer::symbol_table::
    resolve(const semantic_context &context, const definition::fully_qualified_name &full_name,
        const blasphemy::code_position code_pos, const std::optional<std::vector<std::string>::const_iterator> name_end,
        const std::optional<std::vector<std::string>::const_iterator> name_start)
{
    if (full_name.join() == definition::ERROR_IDENTIFIER_CONTENT)
        return std::nullopt;

    const auto name_start_fr = name_start.value_or(full_name.get_parts().begin());
    const auto name_end_fr = name_end.value_or(full_name.get_parts().end());


    if (const auto result = resolve_from_imports(
        context,
        name_start_fr,
        name_end_fr
    ))
    {
        return result.value();
    }


    context.blasphemy_consumer(fmt::format(
        "עֶצֶם בִּלְתִּי־מְזֹהֶה: {}",
        full_name.holy_join()
    ), code_pos);

    return std::nullopt;
}

std::optional<std::reference_wrapper<codesh::semantic_analyzer::symbol>> codesh::semantic_analyzer::symbol_table::
    resolve_from_imports(const semantic_context &context, const std::vector<std::string>::const_iterator name_end,
                         const std::vector<std::string>::const_iterator name_start)
{
    for (const auto &country : context.lookup_countries)
    {
        const auto result = resolve_method_from_scope_container(
            country,
            name_start,
            name_end
        );

        if (result.has_value())
            return result.value();
    }

    return std::nullopt;
}

codesh::semantic_analyzer::named_symbol_map &codesh::semantic_analyzer::symbol_table::get_scope()
{
    return scope;
}

static std::optional<std::reference_wrapper<codesh::semantic_analyzer::symbol>> resolve_method_from_scope_container(
        const codesh::semantic_analyzer::i_scope_containing_symbol &scope_container,
        const std::vector<std::string>::const_iterator fqn_start,
        const std::vector<std::string>::const_iterator fqn_end)
{
    auto it = fqn_start;
    const auto end = fqn_end;

    if (it == end)
        return std::nullopt;

    // We can only resolve for named symbol maps.
    // If the container is not a named symbol map - it means that we've hit a local scope (i.e a method),
    // in which case it is unnamed and thus unaccessible anyway.
    const auto &named_scope_container = dynamic_cast<const codesh::semantic_analyzer::named_symbol_map *>(
        &scope_container.get_scope()
    );
    if (named_scope_container == nullptr)
        return std::nullopt;

    const auto symbol_raw = named_scope_container->resolve_local(*it);
    if (!symbol_raw.has_value())
        return std::nullopt;

    // Successfully consumed the FQN part
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


    // There are still more FQN parts, yet we can't go to them.
    // This means that it simply doesn't exist.
    return std::nullopt;
}

const std::vector<codesh::semantic_analyzer::symbol_type> codesh::semantic_analyzer::symbol_table::ALLOWED_SYMBOL_TYPES = {
    symbol_type::COUNTRY,
    // For the global scope
    symbol_type::TYPE
};
