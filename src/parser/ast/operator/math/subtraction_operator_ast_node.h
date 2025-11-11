#pragma once

#include "../../impl/binary_ast_node.h"

namespace codesh::ast::op
{

class subtraction_operator_ast_node final : public impl::binary_ast_node
{
public:
    subtraction_operator_ast_node(std::unique_ptr<ir_convertable_ast_node> left, std::unique_ptr<ir_convertable_ast_node> right);

    [[nodiscard]] void *to_ir() const override;
};

}

