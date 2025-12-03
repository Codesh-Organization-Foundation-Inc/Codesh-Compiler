#pragma once

#include "ir_emitting_ast_node.h"

#include <memory>

namespace codesh::ast::impl
{

class unary_ast_node : public ir_emitting_ast_node
{
    std::unique_ptr<ir_emitting_ast_node> child;

protected:
    explicit unary_ast_node(std::unique_ptr<ir_emitting_ast_node> child);

public:
    [[nodiscard]] ir_emitting_ast_node *get_child() const;
};

}
