#include "symbol_table.h"

#include "blasphemy/blasphemy_consumer.h"
#include "classpath/loader/class_directory_loader.h"
#include "classpath/loader/class_file_loader.h"
#include "defenition/definitions.h"
#include "lexer/source_file_info.h"
#include "semantic_analyzer/semantic_context.h"

#include <filesystem>
#include <iostream>
#include <ranges>

[[nodiscard]] static std::optional<std::reference_wrapper<codesh::semantic_analyzer::symbol>>
    resolve_method_from_scope_container(
        const codesh::semantic_analyzer::i_scope_containing_symbol &scope_container,
        std::vector<std::string>::const_iterator fqn_start,
        std::vector<std::string>::const_iterator fqn_end);

codesh::semantic_analyzer::symbol_table::symbol_table(
        const definition::class_loaders &class_loaders,
        std::vector<std::string> default_country_lookups) :
    scope(ALLOWED_SYMBOL_TYPES),
    default_imports(std::move(default_country_lookups)),
    class_loaders(class_loaders)
{
    global_country = &scope.add_symbol(
        "",
        std::make_unique<country_symbol>(definition::fully_qualified_name(lexer::NO_CODE_POS))
    ).first.get();

    talmud_codesh_country = &add_talmud_codesh_country();
}

codesh::semantic_analyzer::country_symbol &codesh::semantic_analyzer::symbol_table::get_global_country() const
{
    return *global_country;
}

codesh::semantic_analyzer::country_symbol &codesh::semantic_analyzer::symbol_table::get_talmud_codesh_country() const
{
    return *talmud_codesh_country;
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
        const std::optional<std::vector<std::string>::const_iterator> name_end,
        const std::optional<std::vector<std::string>::const_iterator> name_start) const
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

    const definition::fully_qualified_name name(lexer::NO_CODE_POS, name_start_fr, name_end_fr);
    if (const auto result = try_load_external_symbols(context, name))
    {
        return result.value();
    }


    context.throw_blasphemy(fmt::format(
        "עֶצֶם בִּלְתִּי־מְזֹהֶה: {}",
        full_name.holy_join()
    ), full_name.get_source_range());

    return std::nullopt;
}

std::optional<std::reference_wrapper<codesh::semantic_analyzer::symbol>> codesh::semantic_analyzer::symbol_table::
    resolve_from_imports(const semantic_context &context, const std::vector<std::string>::const_iterator name_start,
                         const std::vector<std::string>::const_iterator name_end)
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

codesh::semantic_analyzer::country_symbol &codesh::semantic_analyzer::symbol_table::add_talmud_codesh_country() const
{
    auto &israel = add_nested_country(get_global_country(), "ישראל", "ישראל");
    auto &codesh = add_nested_country(israel, "קודש","ישראל/קודש");
    auto &ben = add_nested_country(codesh, "בן", "ישראל/קודש/בן");
    auto &moshe = add_nested_country(ben, "משה", "ישראל/קודש/בן/משה");

    return moshe;
}

codesh::semantic_analyzer::country_symbol &codesh::semantic_analyzer::symbol_table::add_nested_country(
    country_symbol &parent, const std::string &name, const std::string &bin_fqn)
{
    return parent.get_scope().add_symbol(
        name,
        std::make_unique<country_symbol>(
            definition::fully_qualified_name::parse(bin_fqn, lexer::NO_CODE_POS),
            &parent
        )
    ).first.get();
}

std::optional<std::reference_wrapper<codesh::semantic_analyzer::symbol>> codesh::semantic_analyzer::symbol_table::
    try_load_external_symbols(const semantic_context &context, const definition::fully_qualified_name &name) const
{
    const auto load_result = try_load_any_candidate(context, name);
    if (!load_result)
        return std::nullopt;

    const auto &[loaded_name, name_suffix] = load_result.value();

    const auto loaded_symbol = resolve_loaded_symbol(context, loaded_name);
    if (!loaded_symbol)
        return std::nullopt;

    if (!name_suffix.has_value())
        return loaded_symbol;

    const auto *inner_scope = dynamic_cast<const i_scope_containing_symbol *>(&loaded_symbol->get());
    if (!inner_scope)
        return std::nullopt;

    const auto &suffix_parts = name_suffix->get_parts();
    return resolve_method_from_scope_container(*inner_scope, suffix_parts.begin(), suffix_parts.end());
}

std::optional<std::reference_wrapper<codesh::semantic_analyzer::symbol>> codesh::semantic_analyzer::symbol_table::
    resolve_loaded_symbol(const semantic_context &context, const definition::fully_qualified_name &name) const
{
    const auto &loaded_name_parts = name.get_parts();

    // Do imports first as it is most likely
    if (const auto result = resolve_from_imports(
        context,
        loaded_name_parts.begin(),
        loaded_name_parts.end()
    ))
    {
        return result;
    }

    // Then from the global scope
    if (const auto result = resolve_method_from_scope_container(
        *global_country,
        loaded_name_parts.begin(),
        loaded_name_parts.end())
    )
    {
        return result;
    }

    //TODO: Throw Codesh warning instead
    std::cout << "Loaded a broken class file" << std::endl;
    return std::nullopt;
}

std::optional<codesh::semantic_analyzer::split_fqn> codesh::semantic_analyzer::symbol_table::try_load_prefixes(
        const std::string &import_prefix, const definition::fully_qualified_name &name) const
{
    const auto &parts = name.get_parts();

    for (auto end = parts.end(); end != parts.begin(); --end)
    {
        definition::fully_qualified_name prefix(lexer::NO_CODE_POS, parts.begin(), end);

        const auto candidate = definition::fully_qualified_name::parse(
            import_prefix.empty()
                ? prefix.join()
                : import_prefix + "/" + prefix.join(),

            lexer::NO_CODE_POS
        );

        if (try_load_candidate(candidate))
        {
            std::optional<definition::fully_qualified_name> suffix;
            if (end != name.get_parts().end())
            {
                suffix.emplace(lexer::NO_CODE_POS, end, name.get_parts().end());
            }

            return std::pair{candidate, std::move(suffix)};
        }
    }

    return std::nullopt;
}

std::optional<codesh::semantic_analyzer::split_fqn> codesh::semantic_analyzer::symbol_table::try_load_any_candidate(
        const semantic_context &context, const definition::fully_qualified_name &name) const
{
    // 1. Direct absolute FQN (programmer wrote "java/lang/String" explicitly)
    if (const auto result = try_load_prefixes("", name))
        return result;

    // 2. Default imports (java/lang, Talmud Codesh)
    for (const auto &import : default_imports)
    {
        if (const auto result = try_load_prefixes(import, name))
            return result;
    }

    // 3. Explicit imports from the source file
    for (const auto &country : context.lookup_countries)
    {
        if (const auto result = try_load_prefixes(country.get().get_full_name().join(), name))
            return result;
    }

    return std::nullopt;
}

bool codesh::semantic_analyzer::symbol_table::try_load_candidate(const definition::fully_qualified_name &candidate)
    const
{
    for (const auto &class_loader : class_loaders | std::views::values)
    {
        if (class_loader->load(*this, candidate))
            return true;
    }

    return false;
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

    const auto symbol_raw = scope_container.resolve_own(*it);
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
