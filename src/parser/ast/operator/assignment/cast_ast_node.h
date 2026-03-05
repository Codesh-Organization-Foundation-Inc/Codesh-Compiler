#pragma once
#include "parser/ast/impl/binary_ast_node.h"
#include "parser/ast/var_reference/variable_reference_ast_node.h"

namespace codesh::ast::op::assignment
{

class cast_ast_node : public impl::binary_ast_node
{
    std::unique_ptr<variable_reference_ast_node> value;
    std::unique_ptr<type::type_ast_node> type;

public:
    cast_ast_node(
        std::unique_ptr<variable_reference_ast_node> value,
        std::unique_ptr<type::type_ast_node> target_type);

    variable_reference_ast_node& get_value();
    const variable_reference_ast_node& get_value() const;

    type::type_ast_node& get_target_type();
    const type::type_ast_node& get_target_type() const;
};

}
