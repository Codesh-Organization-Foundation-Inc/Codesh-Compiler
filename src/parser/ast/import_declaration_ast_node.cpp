#include "import_declaration_ast_node.h"

#include <utility>

codesh::ast::import_declaration_ast_node::import_declaration_ast_node(std::string package_name, const bool is_on_demand,
                                                                      const bool is_static)
    : package_name(std::move(package_name)), is_on_demand(is_on_demand), is_static(is_static)
{
}

std::string codesh::ast::import_declaration_ast_node::get_package_name() const
{
    return this->package_name;
}

bool codesh::ast::import_declaration_ast_node::get_is_on_demand() const
{
    return this->is_on_demand;
}

bool codesh::ast::import_declaration_ast_node::get_is_static() const
{
    return this->is_static;
}

