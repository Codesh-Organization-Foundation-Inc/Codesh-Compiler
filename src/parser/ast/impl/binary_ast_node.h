#pragma once

#include "i_ir_emitter.h"
#include "../var_reference/value_ast_node.h"

#include <memory>

namespace codesh::ast::impl
{

class binary_ast_node : public var_reference::value_ast_node
{
    std::unique_ptr<i_ir_emitter> left, right;
    std::unique_ptr<type::type_ast_node> type;

protected:
    binary_ast_node(std::unique_ptr<i_ir_emitter> left, std::unique_ptr<i_ir_emitter> right);

public:
    [[nodiscard]] i_ir_emitter *get_left() const;
    [[nodiscard]] i_ir_emitter *get_right() const;

    void set_left(std::unique_ptr<i_ir_emitter> node);
    void set_right(std::unique_ptr<i_ir_emitter> node);

    [[nodiscard]] type::type_ast_node *get_type() const override;
};


}

