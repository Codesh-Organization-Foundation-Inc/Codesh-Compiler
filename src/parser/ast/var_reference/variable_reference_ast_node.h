#pragma once

#include "../../../defenition/fully_qualified_class_name.h"
#include "../impl/i_resolvable.h"
#include "value_ast_node.h"

namespace codesh::semantic_analyzer
{
class variable_symbol;
class type_symbol;
}

class variable_reference_ast_node : public codesh::ast::var_reference::value_ast_node,
        codesh::ast::impl::i_resolvable<codesh::semantic_analyzer::variable_symbol>
{
    const codesh::definition::fully_qualified_class_name name;
    std::optional<std::reference_wrapper<codesh::semantic_analyzer::variable_symbol>> resolved_symbol;

protected:
    [[nodiscard]] const std::optional<std::reference_wrapper<codesh::semantic_analyzer::variable_symbol>> &
    _get_resolved() const override;

public:
    [[nodiscard]] const codesh::definition::fully_qualified_class_name &get_unresolved_name() const override;
    void set_resolved(codesh::semantic_analyzer::variable_symbol &symbol) override;

    [[nodiscard]] codesh::ast::type::type_ast_node *get_type() const override;

    explicit variable_reference_ast_node(codesh::definition::fully_qualified_class_name name);
};
