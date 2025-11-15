#include "custom_type_ast_node.h"

std::string codesh::ast::type::custom_type_ast_node::generate_descriptor() const
{
    //TODO: Get classpath from symbol table
    return "L" + name + ";";
}

std::string codesh::ast::type::custom_type_ast_node::get_binary_name() const
{
    //TODO: Get package from symbol table
    return name;
}

std::string codesh::ast::type::custom_type_ast_node::get_name() const
{
    return name;
}

void codesh::ast::type::custom_type_ast_node::set_name(const std::string &name)
{
    this->name = name;
}