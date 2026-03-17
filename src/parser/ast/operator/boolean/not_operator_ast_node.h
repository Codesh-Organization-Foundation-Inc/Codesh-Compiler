#pragma once

#include "parser/ast/impl/unary_ast_node.h"

namespace codesh::ast::op
{

class not_operator_ast_node final : public impl::unary_ast_node
{
public:
    not_operator_ast_node(lexer::code_position code_position, std::unique_ptr<value_ast_node> child);

    [[nodiscard]] std::string to_pretty_string() const override;
};

}
