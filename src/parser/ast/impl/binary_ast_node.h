#pragma once
#include "../var_reference/value_ast_node.h"
#include "ir_emitting_ast_node.h"

#include <memory>

namespace codesh::ast::impl
{

class binary_ast_node : public var_reference::value_ast_node
{
    std::unique_ptr<ir_emitting_ast_node> left, right;

protected:
    binary_ast_node(std::unique_ptr<ir_emitting_ast_node> left, std::unique_ptr<ir_emitting_ast_node> right);

public:
    [[nodiscard]] ir_emitting_ast_node *get_left() const;
    [[nodiscard]] ir_emitting_ast_node *get_right() const;

    void set_left(std::unique_ptr<ir_emitting_ast_node> node);
    void set_right(std::unique_ptr<ir_emitting_ast_node> node);
};


}

