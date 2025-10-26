#pragma once

#include "../../impl/unary_ast_node.h"

namespace codesh::ast::op
{

template <typename T> class not_operator_ast_node final : public impl::unary_ast_node<T>
{
public:
    explicit not_operator_ast_node(std::unique_ptr<impl::typed_ast_node<T>> child);

private:
    [[nodiscard]] T evaluate() const override;
};

}
