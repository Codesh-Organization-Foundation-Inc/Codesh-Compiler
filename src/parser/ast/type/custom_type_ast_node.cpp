#include "custom_type_ast_node.h"

#include <sstream>

codesh::ast::type::custom_type_ast_node::custom_type_ast_node(std::string name) : name(std::move(name))
{
}

std::string codesh::ast::type::custom_type_ast_node::generate_descriptor() const
{
    std::ostringstream builder;

    for (size_t i = 0; i < this->get_array_dimensions(); i++)
    {
        builder << '[';
    }

    builder << "L" << get_binary_name() << ";";

    return builder.str();
}

std::string codesh::ast::type::custom_type_ast_node::get_binary_name() const
{
    //TODO: This should THROW if the name is not resolved.
    return resolved_name.value_or(name);
}

void codesh::ast::type::custom_type_ast_node::set_resolved_name(std::string resolved_name)
{
    this->resolved_name.emplace(std::move(resolved_name));
}

std::string codesh::ast::type::custom_type_ast_node::get_name() const
{
    return name;
}

std::unique_ptr<codesh::ast::type::type_ast_node> codesh::ast::type::custom_type_ast_node::clone() const
{
    return std::make_unique<custom_type_ast_node>(*this);
}