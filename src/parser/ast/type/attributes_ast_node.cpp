#include "attributes_ast_node.h"

codesh::ast::type::attributes_ast_node::attributes_ast_node()
    : visibility(definition::visibility::PACKAGE_PRIVATE), is_static(false), is_final(false), is_abstract(false)
{
}

codesh::definition::visibility codesh::ast::type::attributes_ast_node::get_visibility() const
{
    return visibility;
}
void codesh::ast::type::attributes_ast_node::set_visibility(const definition::visibility visibility)
{
    this->visibility = visibility;
}

bool codesh::ast::type::attributes_ast_node::get_is_static() const
{
    return is_static;
}
void codesh::ast::type::attributes_ast_node::set_is_static(bool is_static)
{
    this->is_static = is_static;
}

bool codesh::ast::type::attributes_ast_node::get_is_final() const
{
    return is_final;
}
void codesh::ast::type::attributes_ast_node::set_is_final(bool is_final)
{
    this->is_final = is_final;
}

bool codesh::ast::type::attributes_ast_node::get_is_abstract() const
{
    return is_abstract;
}
void codesh::ast::type::attributes_ast_node::set_is_abstract(bool is_abstract)
{
    this->is_abstract = is_abstract;
}
