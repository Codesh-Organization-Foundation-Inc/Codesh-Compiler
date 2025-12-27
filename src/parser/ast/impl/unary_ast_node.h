#pragma once

#include "i_ir_emitter.h"
#include "../var_reference/value_ast_node.h"

#include <memory>

namespace codesh::ast::impl
{

class unary_ast_node : public var_reference::value_ast_node
{
    std::unique_ptr<i_ir_emitter> child;

protected:
    explicit unary_ast_node(std::unique_ptr<i_ir_emitter> child);

public:
    [[nodiscard]] i_ir_emitter *get_child() const;
};

}
