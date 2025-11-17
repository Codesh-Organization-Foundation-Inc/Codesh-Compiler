#include "class_declaration_ast_node.h"

codesh::ast::type_decl::class_declaration_ast_node::class_declaration_ast_node(const std::string &name)
    : type_declaration_ast_node(name)
{
}

codesh::ast::type::custom_type_ast_node *codesh::ast::type_decl::class_declaration_ast_node::
    get_super_class() const
{
    return super_class.get();
}

void codesh::ast::type_decl::class_declaration_ast_node::set_super_class(
    std::unique_ptr<type::custom_type_ast_node> super_class)
{
    this->super_class = std::move(super_class);
}

std::list<std::unique_ptr<codesh::ast::method_declaration_ast_node>> &codesh::ast::type_decl::
    class_declaration_ast_node::get_methods()
{
    return methods;
}

const std::list<std::unique_ptr<codesh::ast::method_declaration_ast_node>> &codesh::ast::type_decl::
    class_declaration_ast_node::get_methods() const
{
    return methods;
}

std::list<std::unique_ptr<codesh::ast::constructor_declaration_ast_node>> &codesh::ast::type_decl::
    class_declaration_ast_node::get_constructors()
{
    return constructors;
}

const std::list<std::unique_ptr<codesh::ast::constructor_declaration_ast_node>> &codesh::ast::type_decl::
    class_declaration_ast_node::get_constructors() const
{
    return constructors;
}
