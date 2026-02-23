#include "field_declaration_ast_node.h"

codesh::ast::type_decl::field_declaration_ast_node::field_declaration_ast_node(const blasphemy::code_position pos) :
    variable_declaration_ast_node(pos)
{
}

codesh::ast::var_reference::value_ast_node* codesh::ast::type_decl::field_declaration_ast_node::get_value() const
{
    return value.get();
}

void codesh::ast::type_decl::field_declaration_ast_node::set_value(std::unique_ptr<var_reference::value_ast_node> val)
{
    value = std::move(val);
}

std::string codesh::ast::type_decl::field_declaration_ast_node::generate_descriptor(const bool resolved) const
{
    return get_type()->generate_descriptor(resolved);
}
