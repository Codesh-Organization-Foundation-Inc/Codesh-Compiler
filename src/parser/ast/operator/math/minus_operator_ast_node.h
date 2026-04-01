#pragma once

#include "i_math_operator_holder.h"
#include "parser/ast/impl/unary_ast_node.h"

namespace codesh::ast::op
{

class minus_operator_ast_node final : public impl::unary_ast_node, public i_math_operator_holder
{
public:
    minus_operator_ast_node(lexer::code_position code_position, std::unique_ptr<value_ast_node> child);

    [[nodiscard]] std::string to_pretty_string() const override;

    [[nodiscard]] output::ir::operator_type get_ir_operator_type() const override;
};

}
