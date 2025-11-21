#pragma once

#include "symbol.h"

namespace codesh::semantic_analyzer
{

class symbol_table final : public i_scope_containing_symbol
{
    static const std::vector<symbol_type> ALLOWED_SYMBOL_TYPES;
    named_scope_map global_scope;

protected:
    [[nodiscard]] std::vector<symbol_type> allowed_symbol_types() const override;

public:
    [[nodiscard]] std::optional<std::reference_wrapper<country_symbol>> resolve_country(const std::string &name) const;

    [[nodiscard]] const named_scope_map &get_symbol_map() const override;
    [[nodiscard]] named_scope_map &get_symbol_map() override;
};

}
