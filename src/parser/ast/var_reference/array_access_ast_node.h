#pragma once

#include "parser/ast/var_reference/value_ast_node.h"
#include <memory>

namespace codesh::ast::op
{

class array_access_ast_node : public var_reference::value_ast_node
{
    std::unique_ptr<value_ast_node> array;
    std::unique_ptr<value_ast_node> index;

public:
    array_access_ast_node(blasphemy::code_position code_position, std::unique_ptr<value_ast_node> array,
            std::unique_ptr<value_ast_node> index);

    [[nodiscard]] value_ast_node& get_array() const;
    [[nodiscard]] value_ast_node& get_index() const;

    [[nodiscard]] type::type_ast_node* get_type() const override;
};

}