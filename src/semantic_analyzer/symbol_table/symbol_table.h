#pragma once

#include "blasphemy/blasphemy_collector.h"
#include "semantic_analyzer/external/jimage_loader.h"
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

class symbol_table final : public i_scope_containing_symbol
{
    static const std::vector<symbol_type> ALLOWED_SYMBOL_TYPES;
    named_symbol_map scope;

    /**
     * Imports that will be looked into even if a book did not specify them explictly.
     */
    const std::vector<std::string> default_imports;
    const std::vector<std::filesystem::path> &classpaths;

    static std::optional<std::reference_wrapper<symbol>> resolve_from_imports(const semantic_context &context,
            std::vector<std::string>::const_iterator name_start, std::vector<std::string>::const_iterator name_end);

    country_symbol *global_scope;

    /**
     * Load ALL external symbols related to a type, if one exists
     *
     * @returns The found symbol, @c std::nullopt otherwise
     */
    std::optional<std::reference_wrapper<symbol>> try_load_external_symbols(const semantic_context &context,
            const definition::fully_qualified_name &name) const;

    [[nodiscard]] bool try_load_candidate(const std::string &candidate) const;

public:
    symbol_table(const std::vector<std::filesystem::path> &classpaths,
            std::vector<std::string> default_country_lookups);

    [[nodiscard]] country_symbol &get_global_scope() const;

    [[nodiscard]] std::optional<std::reference_wrapper<country_symbol>> resolve_country(const std::string &name) const;

    [[nodiscard]] std::optional<std::reference_wrapper<symbol>> resolve(
            const semantic_context &context, const definition::fully_qualified_name &full_name,
            blasphemy::code_position code_pos,
            //FIXME: They're flipped
            std::optional<std::vector<std::string>::const_iterator> name_end = std::nullopt,
            std::optional<std::vector<std::string>::const_iterator> name_start = std::nullopt) const;

    [[nodiscard]] named_symbol_map &get_scope() override;
    [[nodiscard]] const named_symbol_map &get_scope() const override;
};

}
