#include "variable_declaration_ast_node.h"
#include "parser/ast/type/custom_type_ast_node.h"


codesh::ast::type_decl::variable_declaration_ast_node::variable_declaration_ast_node(
        blasphemy::code_position pos) : ast_node(std::move(pos))
{
}

const std::string& codesh::ast::type_decl::variable_declaration_ast_node::get_name() const
{
    return name;
}

void codesh::ast::type_decl::variable_declaration_ast_node::set_name(const std::string& name)
{
    this->name = name;
}

codesh::ast::type::type_ast_node* codesh::ast::type_decl::variable_declaration_ast_node::get_type() const
{
    return type.get();
}

void codesh::ast::type_decl::variable_declaration_ast_node::set_type(std::unique_ptr<type::type_ast_node> type)
{
    this->type = std::move(type);
}

codesh::ast::type_decl::attributes_ast_node* codesh::ast::type_decl::variable_declaration_ast_node::get_attributes() const
{
    return attributes.get();
}

void codesh::ast::type_decl::variable_declaration_ast_node::set_attributes(
        std::unique_ptr<attributes_ast_node> attributes)
{
    this->attributes = std::move(attributes);
}
