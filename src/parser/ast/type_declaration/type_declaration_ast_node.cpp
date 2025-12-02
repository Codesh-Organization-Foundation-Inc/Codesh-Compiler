#include "type_declaration_ast_node.h"

#include <utility>

std::optional<std::string> &codesh::ast::type_decl::type_declaration_ast_node::get_resolved_name()
{
    return resolved_name;
}

codesh::ast::type_decl::type_declaration_ast_node::type_declaration_ast_node(std::string name) : name(std::move(name))
{
}

std::string codesh::ast::type_decl::type_declaration_ast_node::generate_descriptor(const bool resolved) const
{
    return "L" + get_binary_name(resolved) + ";";
}

const std::optional<std::string> &codesh::ast::type_decl::type_declaration_ast_node::get_resolved_name() const
{
    return resolved_name;
}

std::string codesh::ast::type_decl::type_declaration_ast_node::get_name() const
{
    return this->name;
}

codesh::ast::type::custom_type_ast_node *codesh::ast::type_decl::type_declaration_ast_node::get_super_class() const
{
    return super_class.get();
}

void codesh::ast::type_decl::type_declaration_ast_node::set_super_class(
    std::unique_ptr<type::custom_type_ast_node> super_class)
{
    this->super_class = std::move(super_class);
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

void codesh::ast::type_decl::type_declaration_ast_node::set_constant_pool(
        output::jvm_target::constant_pool constant_pool)
{
    this->constant_pool = std::make_unique<output::jvm_target::constant_pool>(std::move(constant_pool));
}

const std::list<std::unique_ptr<codesh::ast::method::method_declaration_ast_node>> &codesh::ast::type_decl::
    type_declaration_ast_node::get_all_methods() const
{
    return all_methods;
}

void codesh::ast::type_decl::type_declaration_ast_node::add_method(
    std::unique_ptr<method::method_declaration_ast_node> method)
{
    all_methods.push_front(std::move(method));
    methods.push_back(all_methods.front().get());
}

void codesh::ast::type_decl::type_declaration_ast_node::add_method(
    std::unique_ptr<method::constructor_declaration_ast_node> method)
{
    all_methods.push_back(std::move(method));
    constructors.push_back(static_cast<method::constructor_declaration_ast_node *>(all_methods.back().get())); // NOLINT(*-pro-type-static-cast-downcast)
}

const std::list<codesh::ast::method::constructor_declaration_ast_node *> &codesh::ast::type_decl::
    type_declaration_ast_node::get_constructors() const
{
    return constructors;
}

const std::list<codesh::ast::method::method_declaration_ast_node *> &codesh::ast::type_decl::type_declaration_ast_node::
    get_methods() const
{
    return methods;
}

