#include "method_declaration_ast_node.h"

#include "../../../semantic_analyzer/symbol_table/symbol.h"
#include "fmt/xchar.h"
#include "util.h"

#include <ranges>

const std::optional<std::reference_wrapper<codesh::semantic_analyzer::method_symbol>> &codesh::ast::method::
    method_declaration_ast_node::_get_resolved() const
{
    return resolved_symbol;
}

codesh::ast::method::method_declaration_ast_node::method_declaration_ast_node(
    definition::fully_qualified_class_name name) : name(std::move(name))
{
}

std::string codesh::ast::method::method_declaration_ast_node::generate_descriptor(const bool resolved) const
{
    return util::generate_method_descriptor(resolved, *return_type, parameter_types, *attributes);
}

std::string codesh::ast::method::method_declaration_ast_node::generate_parameters_descriptor(const bool resolved) const
{
    return util::generate_parameters_descriptor(resolved, parameter_types, *attributes);
}

void codesh::ast::method::method_declaration_ast_node::set_resolved(semantic_analyzer::method_symbol &symbol)
{
    resolved_symbol.emplace(symbol);
}

const codesh::definition::fully_qualified_class_name &codesh::ast::method::method_declaration_ast_node::
    get_unresolved_name() const
{
    return name;
}

codesh::ast::type_decl::attributes_ast_node *codesh::ast::method::method_declaration_ast_node::get_attributes()
    const
{
    return attributes.get();
}

void codesh::ast::method::method_declaration_ast_node::set_attributes(
    std::unique_ptr<type_decl::attributes_ast_node> attributes)
{
    this->attributes = std::move(attributes);
}

codesh::ast::type::type_ast_node *codesh::ast::method::method_declaration_ast_node::get_return_type() const
{
    return return_type.get();
}

void codesh::ast::method::method_declaration_ast_node::set_return_type(std::unique_ptr<type::type_ast_node> return_type)
{
    this->return_type = std::move(return_type);
}

codesh::ast::method::method_scope_ast_node &codesh::ast::method::method_declaration_ast_node::get_method_scope()
{
    return method_scope;
}

const codesh::ast::method::method_scope_ast_node &codesh::ast::method::method_declaration_ast_node::get_method_scope()
    const
{
    return method_scope;
}

const std::vector<std::reference_wrapper<codesh::ast::local_variable_declaration_ast_node>> &codesh::ast::method::
    method_declaration_ast_node::get_parameters() const
{
    return parameters;
}

void codesh::ast::method::method_declaration_ast_node::add_parameter(
    std::unique_ptr<local_variable_declaration_ast_node> parameter)
{
    parameter_types.emplace_back(*parameter->get_type());
    parameters.emplace_back(*parameter);

    method_scope.add_local_variable(std::move(parameter));
}

const std::list<std::unique_ptr<codesh::ast::type::type_ast_node>> &codesh::ast::method::method_declaration_ast_node::
    get_exceptions_thrown() const
{
    return exceptions_thrown;
}

std::list<std::unique_ptr<codesh::ast::type::type_ast_node>> &codesh::ast::method::method_declaration_ast_node::
    get_exceptions_thrown()
{
    return exceptions_thrown;
}

void codesh::ast::method::method_declaration_ast_node::emit_constants(const compilation_unit_ast_node &root_node,
        output::jvm_target::constant_pool &constant_pool)
{
    constant_pool.goc_name_and_type_info(
        constant_pool.goc_utf8_info(get_last_name(true)),
        constant_pool.goc_utf8_info(generate_descriptor())
    );

    method_scope.emit_constants(root_node, constant_pool);
}
