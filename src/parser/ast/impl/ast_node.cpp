#include "ast_node.h"

codesh::ast::impl::ast_node::ast_node(const lexer::code_position code_position) :
    code_position(code_position)
{
}

codesh::ast::impl::ast_node::~ast_node() = default;

codesh::lexer::code_position codesh::ast::impl::ast_node::get_code_position() const
{
    return code_position;
}
