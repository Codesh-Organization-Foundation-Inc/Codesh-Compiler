#include "type_declaration_ast_node.h"

std::string codesh::ast::type::type_declaration_ast_node::get_name() const
{
    return name;
}

void codesh::ast::type::type_declaration_ast_node::set_name(const std::string &name)
{
    this->name = name;
}
