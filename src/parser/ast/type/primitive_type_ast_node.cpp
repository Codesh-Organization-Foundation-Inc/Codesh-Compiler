#include "primitive_type_ast_node.h"

codesh::ast::type::primitive_type_ast_node::primitive_type_ast_node(const definition::primitive_type type) : type(type)
{
}

codesh::definition::primitive_type codesh::ast::type::primitive_type_ast_node::get_type() const
{
    return type;
}
