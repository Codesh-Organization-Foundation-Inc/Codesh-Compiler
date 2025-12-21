#include "variable_reference_ast_node.h"

variable_reference_ast_node::variable_reference_ast_node(codesh::definition::fully_qualified_class_name name) :
    value_ast_node(nullptr),
    name(std::move(name))
{
}

const codesh::definition::fully_qualified_class_name &variable_reference_ast_node::get_unresolved_name() const
{
    return name;
}
