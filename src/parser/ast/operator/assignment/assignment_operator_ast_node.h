#pragma once

#include "../../impl/binary_ast_node.h"
#include "../../var_reference/variable_reference_ast_node.h"

namespace codesh::ast::op::assignment
{

class assignment_operator_ast_node : public impl::binary_ast_node
{
public:
    assignment_operator_ast_node(std::unique_ptr<variable_reference_ast_node> left, std::unique_ptr<value_ast_node> right);

    [[nodiscard]] variable_reference_ast_node *get_left() const override;

    [[nodiscard]] type::type_ast_node *get_type() const override;
};

}
