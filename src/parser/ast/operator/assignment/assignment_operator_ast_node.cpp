#include "assignment_operator_ast_node.h"

codesh::ast::op::assignment::assignment_operator_ast_node::assignment_operator_ast_node(
        std::unique_ptr<variable_reference_ast_node> left, std::unique_ptr<value_ast_node> right) :
    binary_ast_node(std::move(left), std::move(right))
{
}

variable_reference_ast_node *codesh::ast::op::assignment::assignment_operator_ast_node::get_left() const
{
    return static_cast<variable_reference_ast_node *>(binary_ast_node::get_left()); // NOLINT(*-pro-type-static-cast-downcast)
}

codesh::ast::type::type_ast_node *codesh::ast::op::assignment::assignment_operator_ast_node::get_type() const
{
    // In assignment, the return type is determined by the left operand.
    return get_left()->get_type();
}
