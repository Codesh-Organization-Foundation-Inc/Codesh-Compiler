#pragma once

#include "../var_reference/value_ast_node.h"

#include <memory>

namespace codesh::ast::impl
{

class unary_ast_node : public var_reference::value_ast_node
{
    std::unique_ptr<type::type_ast_node> type;
    std::unique_ptr<value_ast_node> child;

protected:
    explicit unary_ast_node(std::unique_ptr<value_ast_node> child);

public:
    [[nodiscard]] value_ast_node &get_child() const;
    [[nodiscard]] type::type_ast_node *get_type() const override;
};

}
