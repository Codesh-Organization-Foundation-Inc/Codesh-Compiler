#include "type_ast_node.h"

codesh::ast::type::type_ast_node::type_ast_node() : array_dimensions(0)
{
}

int codesh::ast::type::type_ast_node::get_array_dimensions() const
{
    return array_dimensions;
}

void codesh::ast::type::type_ast_node::set_array_dimensions(const int array_dimensions)
{
    this->array_dimensions = array_dimensions;
}
