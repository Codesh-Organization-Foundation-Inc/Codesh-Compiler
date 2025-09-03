#include "block_ast_node.h"

#include "node_type.h"

codesh::ast::node_type codesh::ast::block_ast_node::get_type() const
{
    return node_type::BLOCK;
}
