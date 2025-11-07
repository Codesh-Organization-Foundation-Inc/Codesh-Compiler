#include "type_declaration_ast_node.h"

#include <utility>

codesh::ast::type_delc::type_declaration_ast_node::type_declaration_ast_node(std::string name) : name(std::move(name))
{
}
std::string codesh::ast::type_delc::type_declaration_ast_node::get_name() const
{
    return this->name;
}
codesh::ast::type_delc::attributes_ast_node *codesh::ast::type_delc::type_declaration_ast_node::get_attributes()
    const
{
    return this->attributes.get();
}
void codesh::ast::type_delc::type_declaration_ast_node::set_attributes(
    std::unique_ptr<attributes_ast_node> attributes)
{
    this->attributes = std::move(attributes);
}
