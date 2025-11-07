#pragma once

#include "ir_convertable_ast_node.h"

#include <memory>

namespace codesh::ast::impl
{

class unary_ast_node : public ir_convertable_ast_node
{
    std::unique_ptr<ir_convertable_ast_node> child;

protected:
    explicit unary_ast_node(std::unique_ptr<ir_convertable_ast_node> child);

public:
    [[nodiscard]] ir_convertable_ast_node *get_child() const;
};

}
