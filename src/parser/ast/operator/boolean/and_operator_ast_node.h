#pragma once

#include "boolean_binary_ast_node.h"
#include "parser/ast/impl/binary_ast_node.h"

namespace codesh::ast::op
{

class and_operator_ast_node final : public boolean_binary_ast_node
{
public:
    and_operator_ast_node(blasphemy::code_position code_position, std::unique_ptr<value_ast_node> left,
            std::unique_ptr<value_ast_node> right);

    [[nodiscard]] std::string to_pretty_string() const override;
};

}
