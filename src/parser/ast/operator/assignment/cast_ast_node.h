#pragma once
#include "parser/ast/impl/binary_ast_node.h"
#include "parser/ast/var_reference/variable_reference_ast_node.h"

namespace codesh::ast::op::assignment
{

class cast_ast_node : public impl::binary_ast_node
{
    std::unique_ptr<value_ast_node> left;
    std::unique_ptr<type::type_ast_node> right;

public:
    cast_ast_node(
        std::unique_ptr<value_ast_node> left,
        std::unique_ptr<type::type_ast_node> right);

    [[nodiscard]] value_ast_node& get_left();
    [[nodiscard]] const value_ast_node& get_left() const;

    [[nodiscard]] type::type_ast_node& get_right();
    [[nodiscard]] const type::type_ast_node& get_right() const;
};

}
