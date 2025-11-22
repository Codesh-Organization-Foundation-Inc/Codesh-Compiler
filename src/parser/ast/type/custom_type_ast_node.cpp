#include "custom_type_ast_node.h"

#include <sstream>

std::string codesh::ast::type::custom_type_ast_node::generate_descriptor() const
{
    std::ostringstream builder;

    for (size_t i = 0; i < this->get_array_dimensions(); i++)
    {
        builder << '[';
    }

    //TODO: Get classpath from symbol table
    builder << "L" << name << ";";

    return builder.str();
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

std::unique_ptr<codesh::ast::type::type_ast_node> codesh::ast::type::custom_type_ast_node::clone() const
{
    return std::make_unique<custom_type_ast_node>(*this);
}