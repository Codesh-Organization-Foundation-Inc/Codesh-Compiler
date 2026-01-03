#pragma once

#include "../var_reference/value_ast_node.h"

#include <memory>

namespace codesh::ast::impl
{

class binary_ast_node : public var_reference::value_ast_node
{
    const std::unique_ptr<value_ast_node> left;
    const std::unique_ptr<value_ast_node> right;

    //FIXME: REMOVE THIS!!
    const std::unique_ptr<type::type_ast_node> type;

protected:
    binary_ast_node(std::unique_ptr<value_ast_node> left, std::unique_ptr<value_ast_node> right);

public:
    [[nodiscard]] virtual value_ast_node *get_left() const;
    [[nodiscard]] value_ast_node *get_right() const;

    //FIXME: REMOVE THIS!!
    // Each node should set the type for itself.
    [[nodiscard]] type::type_ast_node *get_type() const override;
};


}
