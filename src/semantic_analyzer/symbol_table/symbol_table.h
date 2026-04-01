#pragma once

#include "../../classpath/loader/jimage_loader.h"
#include "defenition/definitions.h"
#include "symbol.h"

#include <string>

namespace codesh::ast
{
class compilation_unit_ast_node;
}

namespace codesh::semantic_analyzer
{
struct semantic_context;
}

namespace codesh::semantic_analyzer
{

/**
 * Represents an FQN that was split into a prefix and a suffix, if one exists.
 *
 * If a suffix does not exist, then the original name is the same as the prefix.
 */
using split_fqn = std::pair<
    definition::fully_qualified_name,
    std::optional<definition::fully_qualified_name>
>;

class symbol_table final : public i_scope_containing_symbol
{
    static const std::vector<symbol_type> ALLOWED_SYMBOL_TYPES;
    named_symbol_map scope;

    /**
     * Imports that will be looked into even if a book did not specify them explicitly.
     */
    const std::vector<definition::fully_qualified_name> default_imports;
    const definition::class_loaders &class_loaders;

    static std::optional<std::reference_wrapper<symbol>> resolve_from_imports(const semantic_context &context,
            std::vector<std::string>::const_iterator name_start, std::vector<std::string>::const_iterator name_end);

    country_symbol *global_country;
    country_symbol *talmud_codesh_country;

    std::vector<std::reference_wrapper<type_symbol>> main_classes;

    /**
     * Load ALL external symbols related to a type, if one exists
     *
     * @returns The found symbol, @c std::nullopt otherwise
     */
    std::optional<std::reference_wrapper<symbol>> try_load_external_symbols(const semantic_context &context,
            const definition::fully_qualified_name &name) const;

    std::optional<std::reference_wrapper<symbol>> resolve_loaded_symbol(const semantic_context &context,
            const definition::fully_qualified_name &name) const;

    /**
     * @return The prefix and suffix of the split name, or @c nullptr if the loading was not successful.
     *
     * The FQN will be split if an earlier part of the name was loadable.
     *
     * For instance, calling @c System.out.println will result in the name @c System/out being passed.
     * In that case, the return value will be a @c System suffix (what was loaded) and @c out (what was omitted).
     */
    [[nodiscard]] std::optional<split_fqn> try_load_prefixes(
            const definition::fully_qualified_name &import_prefix, const definition::fully_qualified_name &name) const;
    [[nodiscard]] std::optional<split_fqn> try_load_any_candidate(const semantic_context &context,
            const definition::fully_qualified_name &name) const;

public:
    symbol_table(const definition::class_loaders &class_loaders,
            std::vector<definition::fully_qualified_name> default_country_lookups);

    [[nodiscard]] country_symbol &get_global_country() const;
    [[nodiscard]] country_symbol &get_talmud_codesh_country() const;
    [[nodiscard]] const std::vector<std::reference_wrapper<type_symbol>> &get_main_classes() const;
    void add_main_class(type_symbol &type);
    [[nodiscard]] const std::vector<definition::fully_qualified_name> &get_default_imports() const;

    [[nodiscard]] std::optional<std::reference_wrapper<country_symbol>> resolve_country(const std::string &name) const;

    [[nodiscard]] std::optional<std::reference_wrapper<symbol>> resolve(
            const semantic_context &context, const definition::fully_qualified_name &full_name,
            //FIXME: They're flipped
            std::optional<std::vector<std::string>::const_iterator> name_end = std::nullopt,
            std::optional<std::vector<std::string>::const_iterator> name_start = std::nullopt) const;

    bool try_load_candidate(const definition::fully_qualified_name &candidate) const;

    [[nodiscard]] named_symbol_map &get_scope() override;
    [[nodiscard]] const named_symbol_map &get_scope() const override;
};

}
