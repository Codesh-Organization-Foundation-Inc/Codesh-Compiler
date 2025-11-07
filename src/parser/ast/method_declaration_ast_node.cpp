#include "method_declaration_ast_node.h"

std::string codesh::ast::method_declaration_ast_node::get_name() const
{
    return name;
}

void codesh::ast::method_declaration_ast_node::set_name(const std::string &name)
{
    this->name = name;
}

codesh::ast::type_decl::attributes_ast_node *codesh::ast::method_declaration_ast_node::get_attributes()
    const
{
    return attributes.get();
}

void codesh::ast::method_declaration_ast_node::set_attributes(
    std::unique_ptr<type_decl::attributes_ast_node> attributes)
{
    this->attributes = std::move(attributes);
}

codesh::ast::type::type_ast_node *codesh::ast::method_declaration_ast_node::get_return_type() const
{
    return return_type.get();
}

void codesh::ast::method_declaration_ast_node::set_return_type(std::unique_ptr<type::type_ast_node> return_type)
{
    this->return_type = std::move(return_type);
}

const std::list<std::unique_ptr<codesh::ast::local_variable_declaration_ast_node>> &codesh::ast::method_declaration_ast_node::
    get_parameter_types() const
{
    return parameter_types;
}

const std::list<std::unique_ptr<codesh::ast::type::type_ast_node>> &codesh::ast::method_declaration_ast_node::
    get_exceptions_thrown() const
{
    return exceptions_thrown;
}

std::list<std::unique_ptr<codesh::ast::local_variable_declaration_ast_node>> &codesh::ast::method_declaration_ast_node::
    get_parameter_types()
{
    return parameter_types;
}

std::list<std::unique_ptr<codesh::ast::type::type_ast_node>> &codesh::ast::method_declaration_ast_node::
    get_exceptions_thrown()
{
    return exceptions_thrown;
}