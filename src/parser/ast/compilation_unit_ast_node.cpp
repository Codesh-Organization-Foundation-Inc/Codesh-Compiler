#include "compilation_unit_ast_node.h"

#include <utility>

#include "node_type.h"

codesh::ast::compilation_unit_ast_node::compilation_unit_ast_node(std::string package_name)
    : package_name(std::move(package_name))
{
}

codesh::ast::node_type codesh::ast::compilation_unit_ast_node::get_type() const
{
    return node_type::COMPILATION_UNIT;
}

std::string codesh::ast::compilation_unit_ast_node::get_package_name() const
{
    return this->package_name;
}

const std::list<std::unique_ptr<codesh::ast::import_declaration_ast_node>> &codesh::ast::compilation_unit_ast_node::
    get_import_declarations() const
{
    return this->import_declarations;
}

const std::list<std::unique_ptr<codesh::ast::type_declaration_ast_node>> &codesh::ast::compilation_unit_ast_node::
    get_type_declarations() const
{
    return this->type_declarations;
}
