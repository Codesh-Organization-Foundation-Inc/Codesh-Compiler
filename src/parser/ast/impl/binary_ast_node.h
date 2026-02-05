#pragma once

#include "parser/ast/var_reference/value_ast_node.h"
#include "i_constant_pool_emitter.h"
#include "operator_ast_node.h"

#include <memory>

namespace codesh::ast::impl
{

class binary_ast_node : public operator_ast_node<2>
{
protected:
    binary_ast_node(blasphemy::code_position code_position, std::unique_ptr<value_ast_node> left,
            std::unique_ptr<value_ast_node> right);

public:
    [[nodiscard]] virtual value_ast_node &get_left() const;
    [[nodiscard]] value_ast_node &get_right() const;

    //TODO: Nodes like ++ should specify that only numerical types are allowed.
    [[nodiscard]] bool is_value_valid() const override;


    [[nodiscard]] type::type_ast_node *get_type() const override;
};


}
