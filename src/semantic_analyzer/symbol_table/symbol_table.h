#pragma once

#include "symbol.h"

namespace codesh::semantic_analyzer
{

class symbol_table final : public i_scope_containing_symbol
{
    static const std::vector<symbol_type> ALLOWED_SYMBOL_TYPES;
    named_scope_map global_scope;

    //TODO:
    // void static collect_countries(ast::compilation_unit_ast_node &root_node);

    //TODO:
    // void static collect_fields(ast::type_decl::class_declaration_ast_node &class_decl);

protected:
    [[nodiscard]] std::vector<symbol_type> allowed_symbol_types() const override;
    [[nodiscard]] named_scope_map &get_symbol_map() override;

public:
    explicit symbol_table(const ast::compilation_unit_ast_node &root_node);

    [[nodiscard]] std::optional<std::reference_wrapper<country_symbol>> resolve_country(const std::string &name) const;

    [[nodiscard]] const named_scope_map &get_symbol_map() const override;
};

}
