#include "local_variable_declaration_ast_node.h"

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

codesh::ast::type_decl::attributes_ast_node *codesh::ast::local_variable_declaration_ast_node::get_attributes() const
{
    return attributes.get();
}

void codesh::ast::local_variable_declaration_ast_node::set_attributes(
    std::unique_ptr<type_decl::attributes_ast_node> attributes)
{
    this->attributes = std::move(attributes);
}

