#pragma once

#include "../../impl/ast_node.h"
#include "../../impl/ir_emitting_ast_node.h"

#include <memory>

namespace codesh::ast
{
class block_ast_node;
}


namespace codesh::ast::method::operation
{

class while_ast_node final : public impl::ast_node
{
    std::unique_ptr<impl::ir_emitting_ast_node> condition;
    // std::unique_ptr<block_ast_node> block;

public:
    explicit while_ast_node(std::unique_ptr<impl::ir_emitting_ast_node> condition);

    [[nodiscard]] impl::ir_emitting_ast_node *get_condition() const;
    // [[nodiscard]] block_ast_node *get_block() const;

    void set_condition(std::unique_ptr<impl::ir_emitting_ast_node> condition);
    // void set_block(std::unique_ptr<block_ast_node> block);
};

}