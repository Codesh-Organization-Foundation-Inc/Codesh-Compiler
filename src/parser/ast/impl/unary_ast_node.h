#pragma once

#include "i_ir_emitter.h"

#include <memory>

namespace codesh::ast::impl
{

class unary_ast_node : public i_ir_emitter
{
    std::unique_ptr<i_ir_emitter> child;

protected:
    explicit unary_ast_node(std::unique_ptr<i_ir_emitter> child);

public:
    [[nodiscard]] i_ir_emitter *get_child() const;
};

}
