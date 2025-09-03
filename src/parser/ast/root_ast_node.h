#pragma once

#include "impl/multichild_ast_node.h"

namespace codesh::ast
{


class root_ast_node final : public impl::multichild_ast_node
{
public:
    [[nodiscard]] node_type get_type() const override;
};


}
