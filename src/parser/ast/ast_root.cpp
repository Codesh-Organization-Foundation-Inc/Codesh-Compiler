#include "ast_root.h"

#include "node_type.h"

codesh::ast::node_type codesh::ast::ast_root::get_type() const
{
    return node_type::AST_ROOT;
}
