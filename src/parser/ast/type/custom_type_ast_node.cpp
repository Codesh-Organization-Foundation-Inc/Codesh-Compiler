#include "custom_type_ast_node.h"

#include <iostream>
#include <sstream>

codesh::ast::type::custom_type_ast_node::custom_type_ast_node(std::string name) : name(std::move(name))
{
}

std::optional<std::string> &codesh::ast::type::custom_type_ast_node::get_resolved_name()
{
    return resolved_name;
}

std::string codesh::ast::type::custom_type_ast_node::generate_descriptor(bool resolved) const
{
    std::ostringstream builder;

    for (size_t i = 0; i < this->get_array_dimensions(); i++)
    {
        builder << '[';
    }

    builder << "L" << get_binary_name(resolved) << ";";

    return builder.str();
}

std::string codesh::ast::type::custom_type_ast_node::get_name() const
{
    return name;
}

const std::optional<std::string> &codesh::ast::type::custom_type_ast_node::get_resolved_name() const
{
    return resolved_name;
}

std::unique_ptr<codesh::ast::type::type_ast_node> codesh::ast::type::custom_type_ast_node::clone() const
{
    return std::make_unique<custom_type_ast_node>(*this);
}