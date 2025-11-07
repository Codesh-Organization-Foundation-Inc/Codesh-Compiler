#include "custom_type_ast_node.h"

std::string codesh::ast::type::custom_type_ast_node::get_name() const
{
    return name;
}

void codesh::ast::type::custom_type_ast_node::set_name(const std::string &name)
{
    this->name = name;
}