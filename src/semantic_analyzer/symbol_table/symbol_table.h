#pragma once

#include "semantic_analyzer/symbol_table/symbol.h"

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

    //TODO:
    // void static collect_countries(ast::compilation_unit_ast_node &root_node);

    //TODO:
    // void static collect_fields(ast::type_decl::class_declaration_ast_node &class_decl);

public:
    explicit symbol_table(const ast::compilation_unit_ast_node &root_node);


    [[nodiscard]] std::optional<std::reference_wrapper<country_symbol>> resolve_country(const std::string &name) const;

    [[nodiscard]] static std::optional<std::reference_wrapper<symbol>> resolve_from_imports(
            const semantic_context &context, const definition::fully_qualified_class_name &full_name,
            std::optional<std::vector<std::string>::const_iterator> name_end = std::nullopt,
            std::optional<std::vector<std::string>::const_iterator> name_start = std::nullopt);

    [[nodiscard]] named_symbol_map &get_scope() override;
    [[nodiscard]] const named_symbol_map &get_scope() const override;
};

}
