#include "block_ast_node.h"

std::list<std::unique_ptr<codesh::ast::impl::ast_node>> &codesh::ast::block_ast_node::get_children()
{
    return children;
}
