#include "custom_type_ast_node.h"

#include "fmt/chrono.h"

#include <iostream>
#include <sstream>

codesh::ast::type::custom_type_ast_node::custom_type_ast_node(definition::fully_qualified_class_name name) :
    name(std::move(name))
{
}

std::optional<codesh::definition::fully_qualified_class_name> &codesh::ast::type::custom_type_ast_node::
    _get_resolved_name()
{
    return resolved_name;
}

std::string codesh::ast::type::custom_type_ast_node::generate_descriptor(const bool resolved) const
{
    std::ostringstream builder;

    for (size_t i = 0; i < this->get_array_dimensions(); i++)
    {
        builder << '[';
    }

    builder << fmt::format(
        "L{};",
        get_name(resolved).join()
    );

    return builder.str();
}

const codesh::definition::fully_qualified_class_name &codesh::ast::type::custom_type_ast_node::get_name() const
{
    return name;
}

const std::optional<codesh::definition::fully_qualified_class_name> &codesh::ast::type::custom_type_ast_node::
    _get_resolved_name() const
{
    return resolved_name;
}

std::unique_ptr<codesh::ast::type::type_ast_node> codesh::ast::type::custom_type_ast_node::clone() const
{
    return std::make_unique<custom_type_ast_node>(*this);
}