#include "field_declaration_ast_node.h"

codesh::ast::type_decl::field_declaration_ast_node::field_declaration_ast_node(const blasphemy::code_position pos,
        definition::fully_qualified_name name, std::unique_ptr<type::type_ast_node> type) :
    ast_node(pos),
    name(std::move(name)),
    type(std::move(type))
{
}

const codesh::definition::fully_qualified_name& codesh::ast::type_decl::field_declaration_ast_node::get_name() const
{
    return name;
}

codesh::ast::type::type_ast_node* codesh::ast::type_decl::field_declaration_ast_node::get_type() const
{
    return type.get();
}

codesh::ast::type_decl::attributes_ast_node* codesh::ast::type_decl::field_declaration_ast_node::get_attributes() const
{
    return attributes.get();
}

void codesh::ast::type_decl::field_declaration_ast_node::set_attributes(std::unique_ptr<attributes_ast_node> attributes)
{
    this->attributes = std::move(attributes);
}

codesh::ast::var_reference::value_ast_node* codesh::ast::type_decl::field_declaration_ast_node::get_value() const
{
    return value.get();
}

void codesh::ast::type_decl::field_declaration_ast_node::set_value(std::unique_ptr<var_reference::value_ast_node> val)
{
    value = std::move(val);
}

std::string codesh::ast::type_decl::field_declaration_ast_node::generate_descriptor(bool resolved) const
{
    return type->generate_descriptor(resolved);
}

void codesh::ast::type_decl::field_declaration_ast_node::emit_constants(const compilation_unit_ast_node&,
        output::jvm_target::constant_pool& constant_pool)
{
}

