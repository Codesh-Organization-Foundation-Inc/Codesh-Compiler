#pragma once

#include "../../impl/ast_node.h"
#include "../../impl/i_ir_emitter.h"

#include <memory>

namespace codesh::ast
{
class block_ast_node;
}


namespace codesh::ast::method::operation
{

class while_ast_node final : public impl::ast_node
{
    std::unique_ptr<impl::i_ir_emitter> condition;
    // std::unique_ptr<block_ast_node> block;

public:
    explicit while_ast_node(std::unique_ptr<impl::i_ir_emitter> condition);

    [[nodiscard]] impl::i_ir_emitter *get_condition() const;
    // [[nodiscard]] block_ast_node *get_block() const;

    void set_condition(std::unique_ptr<impl::i_ir_emitter> condition);
    // void set_block(std::unique_ptr<block_ast_node> block);
};

}