#pragma once

#include "method_ast_node.h"

#include <memory>

namespace codesh::ast::impl
{

class unary_ast_node : public method_ast_node
{
    std::unique_ptr<method_ast_node> child;

protected:
    explicit unary_ast_node(std::unique_ptr<method_ast_node> child);

public:
    [[nodiscard]] method_ast_node *get_child() const;
};

}
