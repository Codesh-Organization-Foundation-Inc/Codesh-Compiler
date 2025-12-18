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
    return fmt::format(
        "{}L{};",
        std::string(get_array_dimensions(), '['),
        get_name(resolved).join()
    );
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