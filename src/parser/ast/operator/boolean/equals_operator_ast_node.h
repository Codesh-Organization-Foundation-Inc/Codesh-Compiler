#pragma once

#include "../../impl/binary_ast_node.h"

namespace codesh::ast::op
{

class equals_operator_ast_node final : public impl::binary_ast_node
{
public:
    equals_operator_ast_node(std::unique_ptr<method_ast_node> left, std::unique_ptr<method_ast_node> right);

    [[nodiscard]] void *to_ir() const override;
};

}

