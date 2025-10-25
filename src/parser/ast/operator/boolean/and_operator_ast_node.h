#pragma once

#include "../../impl/binary_ast_node.h"

namespace codesh::ast::op
{

template <typename T>
class and_operator_ast_node final : public impl::binary_ast_node<T>
{
public:
    and_operator_ast_node(std::unique_ptr<impl::typed_ast_node<T>> left, std::unique_ptr<impl::typed_ast_node<T>> right);

private:
    [[nodiscard]] T evaluate() const override;
};

}

