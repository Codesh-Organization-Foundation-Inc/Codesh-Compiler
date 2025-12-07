#include "import_declaration_ast_node.h"

#include "../../defenition/fully_qualified_class_name.h"

codesh::ast::import_declaration_ast_node::import_declaration_ast_node() :
    is_on_demand(false), is_static(false)
{};

codesh::definition::fully_qualified_class_name &codesh::ast::import_declaration_ast_node::get_package_name()
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

void codesh::ast::import_declaration_ast_node::set_package_name(
        const definition::fully_qualified_class_name &package_name)
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
