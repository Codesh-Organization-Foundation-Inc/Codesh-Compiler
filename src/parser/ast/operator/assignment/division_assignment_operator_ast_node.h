#pragma once
#include "assignment_operator_ast_node.h"

namespace codesh::ast::op::assignment
{
class division_assignment_operator_ast_node final : public assignment_operator_ast_node
{
public:
    division_assignment_operator_ast_node(blasphemy::code_position code_position, std::unique_ptr<variable_reference_ast_node> left, std::unique_ptr<value_ast_node> right);

    [[nodiscard]] std::string to_pretty_string() const override;

    [[nodiscard]] std::optional<output::ir::operator_type> get_operator_type() const override;
};
}
