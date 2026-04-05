#pragma once

#include "parser/ast/impl/unary_ast_node.h"

namespace codesh::ast::op
{

class bitwise_not_ast_node final : public impl::unary_ast_node
{
public:
    bitwise_not_ast_node(lexer::code_position code_position, std::unique_ptr<value_ast_node> child);

    [[nodiscard]] std::string to_pretty_string() const override;
};

}
