#include "compilation_unit_ast_node.h"

#include <utility>

codesh::ast::compilation_unit_ast_node::compilation_unit_ast_node(std::string package_name)
    : package_name(std::move(package_name))
{
}

std::string codesh::ast::compilation_unit_ast_node::get_package_name() const
{
    return this->package_name;
}

std::list<std::unique_ptr<codesh::ast::import_declaration_ast_node>> &codesh::ast::compilation_unit_ast_node::
    get_import_declarations()
{
    return this->import_declarations;
}

std::list<std::unique_ptr<codesh::ast::type_declaration_ast_node>> &codesh::ast::compilation_unit_ast_node::
    get_type_declarations()
{
    return this->type_declarations;
}
