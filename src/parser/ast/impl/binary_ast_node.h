#pragma once

#include "../var_reference/value_ast_node.h"

#include <memory>

namespace codesh::ast::impl
{

class binary_ast_node : public var_reference::value_ast_node
{
    std::unique_ptr<value_ast_node> left, right;
    std::unique_ptr<type::type_ast_node> type;

protected:
    binary_ast_node(std::unique_ptr<value_ast_node> left, std::unique_ptr<value_ast_node> right);

public:
    [[nodiscard]] value_ast_node *get_left() const;
    [[nodiscard]] value_ast_node *get_right() const;

    void set_left(std::unique_ptr<value_ast_node> node);
    void set_right(std::unique_ptr<value_ast_node> node);

    [[nodiscard]] type::type_ast_node *get_type() const override;
};


}

