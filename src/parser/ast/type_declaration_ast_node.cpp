#include "type_declaration_ast_node.h"

#include "node_type.h"

codesh::ast::node_type codesh::ast::type_declaration_ast_node::get_type() const
{
    return node_type::TYPE_DECLARATION;
}