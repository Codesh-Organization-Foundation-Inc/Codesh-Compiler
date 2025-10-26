#include "while_ast_node.h"
#include "block_ast_node.h"

codesh::ast::while_ast_node::while_ast_node(std::unique_ptr<impl::method_ast_node> condition,
                                            std::unique_ptr<block_ast_node> block)
    : condition(std::move(condition)), block(std::move(block))
{
}

codesh::ast::impl::method_ast_node *codesh::ast::while_ast_node::get_condition() const
{
    return this->condition.get();
}

codesh::ast::block_ast_node *codesh::ast::while_ast_node::get_block() const
{
    return this->block.get();
}

void codesh::ast::while_ast_node::set_condition(std::unique_ptr<impl::method_ast_node> condition)
{
    this->condition = std::move(condition);
}

void codesh::ast::while_ast_node::set_block(std::unique_ptr<block_ast_node> block)
{
    this->block = std::move(block);
}
