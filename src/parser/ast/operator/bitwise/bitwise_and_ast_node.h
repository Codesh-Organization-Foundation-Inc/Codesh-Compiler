#pragma once

#include "parser/ast/impl/binary_ast_node.h"

namespace codesh::ast::op
{

class bitwise_and_ast_node final : public impl::binary_ast_node
{
public:
    bitwise_and_ast_node(lexer::code_position code_position, std::unique_ptr<value_ast_node> left,
            std::unique_ptr<value_ast_node> right);

    [[nodiscard]] std::string to_pretty_string() const override;
};

}
