#include "type_declaration_ast_node.h"

#include <utility>

codesh::ast::type_decl::type_declaration_ast_node::type_declaration_ast_node(std::string name) : name(std::move(name))
{
}

std::string codesh::ast::type_decl::type_declaration_ast_node::generate_descriptor() const
{
    return "L" + get_binary_name() + ";";
}

std::string codesh::ast::type_decl::type_declaration_ast_node::get_binary_name() const
{
    //TODO: This should THROW if the name is not resolved.
    return resolved_name.value_or(name);
}

void codesh::ast::type_decl::type_declaration_ast_node::set_resolved_name(std::string resolved_name)
{
    this->resolved_name.emplace(std::move(resolved_name));
}

std::string codesh::ast::type_decl::type_declaration_ast_node::get_name() const
{
    return this->name;
}
codesh::ast::type_decl::attributes_ast_node *codesh::ast::type_decl::type_declaration_ast_node::get_attributes()
    const
{
    return this->attributes.get();
}
void codesh::ast::type_decl::type_declaration_ast_node::set_attributes(
    std::unique_ptr<attributes_ast_node> attributes)
{
    this->attributes = std::move(attributes);
}

std::optional<std::reference_wrapper<const codesh::output::jvm_target::constant_pool>> codesh::ast::type_decl::
    type_declaration_ast_node::get_constant_pool() const
{
    if (constant_pool == nullptr)
        return std::nullopt;

    return *constant_pool;
}

void codesh::ast::type_decl::type_declaration_ast_node::set_constant_pool(output::jvm_target::constant_pool constant_pool)
{
    this->constant_pool = std::make_unique<output::jvm_target::constant_pool>(std::move(constant_pool));
}
