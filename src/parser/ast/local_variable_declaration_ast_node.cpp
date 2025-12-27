#include "local_variable_declaration_ast_node.h"

#include "../../semantic_analyzer/symbol_table/symbol.h"

codesh::ast::local_variable_declaration_ast_node::local_variable_declaration_ast_node() :
    accessible_up_to(-1)
{
}

std::string codesh::ast::local_variable_declaration_ast_node::get_name() const
{
    return name;
}

void codesh::ast::local_variable_declaration_ast_node::set_name(const std::string &name)
{
    this->name = name;
}

codesh::ast::type::type_ast_node *codesh::ast::local_variable_declaration_ast_node::get_type() const
{
    return type.get();
}

void codesh::ast::local_variable_declaration_ast_node::set_type(std::unique_ptr<type::type_ast_node> type)
{
    this->type = std::move(type);
}

codesh::ast::var_reference::value_ast_node *codesh::ast::local_variable_declaration_ast_node::get_value() const
{
    return value.get();
}

void codesh::ast::local_variable_declaration_ast_node::set_value(std::unique_ptr<var_reference::value_ast_node> value)
{
    this->value = std::move(value);
}

codesh::ast::type_decl::attributes_ast_node *codesh::ast::local_variable_declaration_ast_node::get_attributes() const
{
    return attributes.get();
}

void codesh::ast::local_variable_declaration_ast_node::set_attributes(
    std::unique_ptr<type_decl::attributes_ast_node> attributes)
{
    this->attributes = std::move(attributes);
}

int codesh::ast::local_variable_declaration_ast_node::get_accessible_up_to() const
{
    return accessible_up_to;
}

void codesh::ast::local_variable_declaration_ast_node::set_accessible_up_to(const int available_to)
{
    this->accessible_up_to = available_to;
}

void codesh::ast::local_variable_declaration_ast_node::add_to_scope(semantic_analyzer::method_scope_symbol &scope)
{
    scope.add_variable(name, std::make_unique<semantic_analyzer::local_variable_symbol>(
        &scope,
        definition::fully_qualified_class_name(name),
        type->clone(),
        this
    ));
}

void codesh::ast::local_variable_declaration_ast_node::emit_constants(const compilation_unit_ast_node &root_node,
                                                                      output::jvm_target::constant_pool &constant_pool)
{
    constant_pool.goc_utf8_info(get_name());
    constant_pool.goc_utf8_info(get_type()->generate_descriptor());
}
