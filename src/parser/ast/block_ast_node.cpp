#include "block_ast_node.h"

#include "node_type.h"

std::list<std::unique_ptr<codesh::ast::impl::ast_node>> &codesh::ast::block_ast_node::get_children()
{
    return children;
}

codesh::ast::node_type codesh::ast::block_ast_node::get_type() const
{
    return node_type::BLOCK;
}
