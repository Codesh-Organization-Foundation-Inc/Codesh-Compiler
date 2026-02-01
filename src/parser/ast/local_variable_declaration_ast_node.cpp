#include "local_variable_declaration_ast_node.h"

#include "output/jvm_target/constant_pool.h"
#include "semantic_analyzer/symbol_table/symbol.h"

const std::optional<std::reference_wrapper<codesh::semantic_analyzer::local_variable_symbol>> &codesh::ast::
    local_variable_declaration_ast_node::_get_resolved() const
{
    return resolved_variable;
}

codesh::ast::local_variable_declaration_ast_node::local_variable_declaration_ast_node(
        const blasphemy::code_position code_position) :
    ast_node(code_position),
    accessible_from(-1),
    accessible_to(-1)
{
}

void codesh::ast::local_variable_declaration_ast_node::set_resolved(semantic_analyzer::local_variable_symbol &symbol)
{
    resolved_variable.emplace(symbol);
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

codesh::ast::type_decl::attributes_ast_node *codesh::ast::local_variable_declaration_ast_node::get_attributes() const
{
    return attributes.get();
}

void codesh::ast::local_variable_declaration_ast_node::set_attributes(
    std::unique_ptr<type_decl::attributes_ast_node> attributes)
{
    this->attributes = std::move(attributes);
}

size_t codesh::ast::local_variable_declaration_ast_node::get_accessible_to() const
{
    return accessible_to;
}

void codesh::ast::local_variable_declaration_ast_node::set_accessible_to(const size_t accessible_to)
{
    this->accessible_to = accessible_to;
}

size_t codesh::ast::local_variable_declaration_ast_node::get_accessible_from() const
{
    return accessible_from;
}

void codesh::ast::local_variable_declaration_ast_node::set_accessible_from(const size_t accessible_from)
{
    this->accessible_from = accessible_from;
}

void codesh::ast::local_variable_declaration_ast_node::emit_constants(const compilation_unit_ast_node &root_node,
                                                                      output::jvm_target::constant_pool &constant_pool)
{
    constant_pool.goc_utf8_info(get_name());
    constant_pool.goc_utf8_info(get_type()->generate_descriptor());
}
