#pragma once

#include "../../impl/binary_ast_node.h"

namespace codesh::ast::op
{

template <typename T>
class addition_operator_ast_node final : public impl::binary_ast_node<T>
{
public:
    addition_operator_ast_node(std::unique_ptr<typed_ast_node<T>> left, std::unique_ptr<typed_ast_node<T>> right);

    [[nodiscard]] node_type get_type() const override;

private:
    [[nodiscard]] T evaluate() const override;
};

}

