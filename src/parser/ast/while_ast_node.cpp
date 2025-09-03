#include "while_ast_node.h"

#include "node_type.h"

codesh::ast::while_ast_node::while_ast_node(std::unique_ptr<typed_ast_node<bool>> condition,
                                            std::unique_ptr<block_ast_node> block) :
    condition(std::move(condition)),
    block(std::move(block))
{}

codesh::ast::node_type codesh::ast::while_ast_node::get_type() const
{
    return node_type::WHILE;
}
