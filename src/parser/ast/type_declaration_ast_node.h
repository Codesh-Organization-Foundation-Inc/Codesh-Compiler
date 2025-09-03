#pragma once

#include "impl/ast_node.h"

namespace codesh::ast
{

class type_declaration_ast_node final : public impl::ast_node
{


public:
    [[nodiscard]] node_type get_type() const override;
};

}
