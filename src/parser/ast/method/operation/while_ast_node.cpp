#include "while_ast_node.h"

codesh::ast::method::operation::while_ast_node::while_ast_node(std::unique_ptr<impl::i_ir_emitter> condition)
    : condition(std::move(condition))
{
}

codesh::ast::impl::i_ir_emitter *codesh::ast::method::operation::while_ast_node::get_condition() const
{
    return this->condition.get();
}

// codesh::ast::block_ast_node *codesh::ast::method::operation::while_ast_node::get_block() const
// {
//     return this->block.get();
// }

void codesh::ast::method::operation::while_ast_node::set_condition(std::unique_ptr<impl::i_ir_emitter> condition)
{
    this->condition = std::move(condition);
}

// void codesh::ast::method::operation::while_ast_node::set_block(std::unique_ptr<block_ast_node> block)
// {
//     this->block = std::move(block);
// }
