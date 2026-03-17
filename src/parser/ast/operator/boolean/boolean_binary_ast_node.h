#pragma once

#include "parser/ast/impl/binary_ast_node.h"
#include "parser/ast/type/primitive_type_ast_node.h"

namespace codesh::ast::op
{

class boolean_binary_ast_node : public impl::binary_ast_node
{
    const std::unique_ptr<type::primitive_type_ast_node> type;

public:
    boolean_binary_ast_node(const lexer::code_position &code_position,
                            std::unique_ptr<value_ast_node> left, std::unique_ptr<value_ast_node> right);

    [[nodiscard]] type::type_ast_node *get_type() const override;
};

}