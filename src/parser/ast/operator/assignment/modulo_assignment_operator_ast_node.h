#pragma once
#include "assignment_operator_ast_node.h"

namespace codesh::ast::op::assignment
{
class modulo_assignment_operator_ast_node final : public assignment_operator_ast_node
{
public:
    modulo_assignment_operator_ast_node(lexer::code_position code_position,
            std::unique_ptr<var_reference::variable_reference_ast_node> left, std::unique_ptr<value_ast_node> right);

    [[nodiscard]] std::string to_pretty_string() const override;

    [[nodiscard]] output::ir::operator_type get_operator_type() const override;
};
}
