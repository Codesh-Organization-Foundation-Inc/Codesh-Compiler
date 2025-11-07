#pragma once

#include "../../impl/unary_ast_node.h"

namespace codesh::ast::op
{

class not_operator_ast_node final : public impl::unary_ast_node
{
public:
    explicit not_operator_ast_node(std::unique_ptr<ir_convertable_ast_node> child);

    [[nodiscard]] void *to_ir() const override;
};

}
