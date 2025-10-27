#include "import_declaration_ast_node.h"

codesh::ast::import_declaration_ast_node::import_declaration_ast_node() :
    is_on_demand(false), is_static(false)
{};

std::list<std::string> &codesh::ast::import_declaration_ast_node::get_package_name()
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

void codesh::ast::import_declaration_ast_node::set_package_name(const std::list<std::string> &package_name)
{
    this->package_name = package_name;
}
void codesh::ast::import_declaration_ast_node::set_is_on_demand(const bool is_on_demand)
{
    this->is_on_demand = is_on_demand;
}
void codesh::ast::import_declaration_ast_node::set_is_static(const bool is_static)
{
    this->is_static = is_static;
}
