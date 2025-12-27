#pragma once
#include "i_ir_emitter.h"

#include <memory>

namespace codesh::ast::impl
{

class binary_ast_node : public i_ir_emitter
{
    std::unique_ptr<i_ir_emitter> left, right;

protected:
    binary_ast_node(std::unique_ptr<i_ir_emitter> left, std::unique_ptr<i_ir_emitter> right);

public:
    [[nodiscard]] i_ir_emitter *get_left() const;
    [[nodiscard]] i_ir_emitter *get_right() const;

    void set_left(std::unique_ptr<i_ir_emitter> node);
    void set_right(std::unique_ptr<i_ir_emitter> node);
};


}

