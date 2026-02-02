#include "ast_node.h"

codesh::ast::impl::ast_node::ast_node(const blasphemy::code_position code_position) :
    code_position(code_position)
{
}

codesh::ast::impl::ast_node::~ast_node() = default;

codesh::blasphemy::code_position codesh::ast::impl::ast_node::get_code_position() const
{
    return code_position;
}
