#pragma once

#include "parser/ast/impl/i_constant_pool_emitter.h"
#include "parser/ast/var_reference/value_ast_node.h"

namespace codesh::ast::op::assignment
{

class cast_ast_node : public var_reference::value_ast_node, public impl::i_constant_pool_emitter
{
    std::unique_ptr<value_ast_node> value;

protected:
    explicit cast_ast_node(lexer::code_position code_position, std::unique_ptr<value_ast_node> value);

public:
    [[nodiscard]] value_ast_node &get_value() const;
};

}
