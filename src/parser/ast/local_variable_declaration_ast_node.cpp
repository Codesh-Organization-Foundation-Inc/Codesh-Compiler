#include "local_variable_declaration_ast_node.h"

codesh::ast::local_variable_declaration_ast_node::local_variable_declaration_ast_node() : final(false)
{
}

std::string codesh::ast::local_variable_declaration_ast_node::get_name() const
{
    return name;
}

void codesh::ast::local_variable_declaration_ast_node::set_name(const std::string &name)
{
    this->name = name;
}

codesh::ast::type::type_ast_node *codesh::ast::local_variable_declaration_ast_node::get_type() const
{
    return type.get();
}

void codesh::ast::local_variable_declaration_ast_node::set_type(std::unique_ptr<type::type_ast_node> type)
{
    this->type = std::move(type);
}

codesh::ast::var_reference::value_ast_node *codesh::ast::local_variable_declaration_ast_node::get_value() const
{
    return value.get();
}

void codesh::ast::local_variable_declaration_ast_node::set_value(std::unique_ptr<var_reference::value_ast_node> value)
{
    this->value = std::move(value);
}

bool codesh::ast::local_variable_declaration_ast_node::is_final() const
{
    return final;
}

void codesh::ast::local_variable_declaration_ast_node::set_is_final(bool is_final)
{
    this->final = is_final;
}
