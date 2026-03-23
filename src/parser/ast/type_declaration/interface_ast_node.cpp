#include "interface_ast_node.h"

codesh::ast::type_decl::interface_declaration_ast_node::interface_declaration_ast_node(
        lexer::code_position code_position, definition::fully_qualified_name name) :
    ast_node(code_position), name(std::move(name))
{
}

codesh::ast::type_decl::interface_declaration_ast_node::~interface_declaration_ast_node() = default;

void codesh::ast::type_decl::interface_declaration_ast_node::set_resolved(semantic_analyzer::type_symbol &symbol)
{
    resolved_symbol = symbol;
}

const std::optional<std::reference_wrapper<codesh::semantic_analyzer::type_symbol>> &codesh::ast::type_decl::
        interface_declaration_ast_node::_get_resolved() const
{
    return resolved_symbol;
}

const codesh::definition::fully_qualified_name &codesh::ast::type_decl::
        interface_declaration_ast_node::get_unresolved_name() const
{
    return name;
}

std::string codesh::ast::type_decl::interface_declaration_ast_node::generate_descriptor(bool resolved) const
{
    return nullptr;
}

void codesh::ast::type_decl::interface_declaration_ast_node::add_extended_interface(
        std::unique_ptr<type::custom_type_ast_node> interface)
{
    extended_interfaces.push_back(std::move(interface));
}

const std::vector<std::unique_ptr<codesh::ast::type::custom_type_ast_node>> &codesh::ast::type_decl::
        interface_declaration_ast_node::get_extended_interfaces() const
{
    return extended_interfaces;
}

void codesh::ast::type_decl::interface_declaration_ast_node::add_method(
        std::unique_ptr<method::method_declaration_ast_node> method)
{
    methods.push_back(std::move(method));
}

const std::deque<std::unique_ptr<codesh::ast::method::method_declaration_ast_node>> &codesh::ast::type_decl::
        interface_declaration_ast_node::get_methods() const
{
    return methods;
}

void codesh::ast::type_decl::interface_declaration_ast_node::set_attributes(
        std::unique_ptr<attributes_ast_node> attr)
{
    attributes = std::move(attr);
}

codesh::ast::type_decl::attributes_ast_node *codesh::ast::type_decl::
        interface_declaration_ast_node::get_attributes() const
{
    return attributes.get();
}

const codesh::output::jvm_target::constant_pool& codesh::ast::type_decl::interface_declaration_ast_node::
        get_constant_pool() const
{
    return nullptr;
}

void codesh::ast::type_decl::interface_declaration_ast_node::set_constant_pool(
        output::jvm_target::constant_pool constant_pool)
{
}

void codesh::ast::type_decl::interface_declaration_ast_node::emit_constants(const compilation_unit_ast_node& root_node,
        output::jvm_target::constant_pool& constant_pool)
{
}
