#pragma once

#include "increment_decrement_operator_ast_node.h"

namespace codesh::ast::op::assignment
{

class increment_operator_ast_node final : public increment_decrement_operator_ast_node
{
public:
    increment_operator_ast_node(lexer::code_position code_position,
            std::unique_ptr<var_reference::variable_reference_ast_node> variable);

    [[nodiscard]] std::string to_pretty_string() const override;

    [[nodiscard]] output::ir::operator_type get_operator_type() const override;
};

}
