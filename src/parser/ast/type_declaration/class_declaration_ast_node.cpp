#include "class_declaration_ast_node.h"

codesh::ast::type_decl::class_declaration_ast_node::class_declaration_ast_node(const std::string &name)
    : type_declaration_ast_node(name)
{
}

std::list<std::unique_ptr<codesh::ast::method::method_declaration_ast_node>> &codesh::ast::type_decl::
    class_declaration_ast_node::get_methods()
{
    return methods;
}

const std::list<std::unique_ptr<codesh::ast::method::method_declaration_ast_node>> &codesh::ast::type_decl::
    class_declaration_ast_node::get_methods() const
{
    return methods;
}
