#pragma once

#include "blasphemy/blasphemy_collector.h"
#include "symbol.h"

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

    const std::vector<std::filesystem::path> &classpaths;

    static std::optional<std::reference_wrapper<symbol>> resolve_from_imports(const semantic_context &context,
            std::vector<std::string>::const_iterator name_start, std::vector<std::string>::const_iterator name_end);

    /**
     * Load ALL external symbols related to a type, if one exists
     *
     * @returns The found symbol, @c std::nullopt otherwise
     */
    std::optional<std::reference_wrapper<symbol>> try_load_external_symbols(const semantic_context &context,
            const definition::fully_qualified_name &name) const;

public:
    explicit symbol_table(const std::vector<std::filesystem::path> &classpaths);

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
