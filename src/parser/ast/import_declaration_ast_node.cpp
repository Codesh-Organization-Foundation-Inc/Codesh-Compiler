#include "import_declaration_ast_node.h"


codesh::ast::import_declaration_ast_node::import_declaration_ast_node(const lexer::code_position code_position) :
    ast_node(code_position), package_name(code_position), is_static(false)
{}

codesh::definition::fully_qualified_name &codesh::ast::import_declaration_ast_node::get_package_name()
{
    return this->package_name;
}

const codesh::definition::fully_qualified_name &codesh::ast::import_declaration_ast_node::get_package_name() const
{
    return this->package_name;
}

bool codesh::ast::import_declaration_ast_node::get_is_static() const
{
    return this->is_static;
}

void codesh::ast::import_declaration_ast_node::set_is_static(const bool is_static)
{
    this->is_static = is_static;
}
