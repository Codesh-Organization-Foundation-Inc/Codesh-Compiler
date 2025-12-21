#include "reference_value_ast_node.h"

reference_value_ast_node::reference_value_ast_node(codesh::definition::fully_qualified_class_name name) :
    value_ast_node(nullptr),
    name(std::move(name))
{
}

const codesh::definition::fully_qualified_class_name &reference_value_ast_node::get_unresolved_name() const
{
    return name;
}
