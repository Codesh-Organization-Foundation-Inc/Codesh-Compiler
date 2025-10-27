#include "compilation_unit_ast_node.h"

codesh::ast::compilation_unit_ast_node::compilation_unit_ast_node(const basad_type basad_type)
    : basad(basad_type)
{
}

basad_type codesh::ast::compilation_unit_ast_node::get_basad_type() const
{
    return this->basad;
}

std::list<std::string> &codesh::ast::compilation_unit_ast_node::get_package_name()
{
    return this->package_name;
}

std::list<std::unique_ptr<codesh::ast::import_declaration_ast_node>> &codesh::ast::compilation_unit_ast_node::
    get_import_declarations()
{
    return this->import_declarations;
}

std::list<std::unique_ptr<codesh::ast::type::type_declaration_ast_node>> &codesh::ast::compilation_unit_ast_node::
    get_type_declarations()
{
    return this->type_declarations;
}
