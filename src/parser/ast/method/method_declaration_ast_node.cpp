#include "method_declaration_ast_node.h"

#include "../local_variable_declaration_ast_node.h"
#include "fmt/xchar.h"

#include <ranges>

std::string codesh::ast::method::method_declaration_ast_node::generate_descriptor(const bool resolved) const
{
    // In JVM, a method descriptor is defined as such:
    // (args...)return_type

    return fmt::format(
        "({}){}",
        generate_parameters_descriptor(resolved),
        get_return_type()->generate_descriptor(resolved)
    );
}

std::string codesh::ast::method::method_declaration_ast_node::generate_parameters_descriptor(const bool resolved) const
{
    if (!resolved)
        return generate_unresolved_parameter_descriptors();

    fmt::memory_buffer out;
    const auto &params = get_parameters();

    auto it = params.begin();

    // If the method isn't static, skip the first parameter ('this').
    if (!attributes->get_is_static()) {
        ++it;
    }

    for (; it != params.end(); ++it)
    {
        const auto &param = **it;

        fmt::format_to(
            std::back_inserter(out),
            "{}",
            param.get_type()->generate_descriptor()
        );
    }

    return fmt::to_string(out);
}

std::string codesh::ast::method::method_declaration_ast_node::generate_unresolved_parameter_descriptors() const
{
    auto descriptors = get_parameters()
        | std::views::transform([](const std::unique_ptr<local_variable_declaration_ast_node> &var) {
            return var->get_type()->generate_descriptor(false);
        });

    return fmt::format(
        "{}",
        fmt::join(descriptors, "")
    );
}

std::string codesh::ast::method::method_declaration_ast_node::get_name() const
{
    return name;
}

void codesh::ast::method::method_declaration_ast_node::set_name(const std::string &name)
{
    this->name = name;
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

codesh::semantic_analyzer::method_symbol &codesh::ast::method::method_declaration_ast_node::get_symbol() const
{
    if (!symbol)
        throw std::runtime_error("No symbol attached to the method declaration node");

    return symbol.value();
}

void codesh::ast::method::method_declaration_ast_node::set_symbol(semantic_analyzer::method_symbol &symbol)
{
    this->symbol.emplace(symbol);
}

std::list<std::unique_ptr<codesh::ast::impl::ir_emitting_ast_node>> &codesh::ast::method::method_declaration_ast_node::
    get_body()
{
    return body;
}

const std::list<std::unique_ptr<codesh::ast::impl::ir_emitting_ast_node>> &codesh::ast::method::method_declaration_ast_node::
    get_body() const
{
    return body;
}
std::list<std::unique_ptr<codesh::ast::local_variable_declaration_ast_node>> &codesh::ast::method::
    method_declaration_ast_node::get_local_variables()
{
    return local_variables;
}

const std::list<std::unique_ptr<codesh::ast::local_variable_declaration_ast_node>> &codesh::ast::method::
    method_declaration_ast_node::get_local_variables() const
{
    return local_variables;
}

const std::list<std::unique_ptr<codesh::ast::local_variable_declaration_ast_node>> &codesh::ast::method::method_declaration_ast_node::
    get_parameters() const
{
    return parameters;
}

const std::list<std::unique_ptr<codesh::ast::type::type_ast_node>> &codesh::ast::method::method_declaration_ast_node::
    get_exceptions_thrown() const
{
    return exceptions_thrown;
}

std::list<std::unique_ptr<codesh::ast::local_variable_declaration_ast_node>> &codesh::ast::method::method_declaration_ast_node::
    get_parameters()
{
    return parameters;
}

std::list<std::unique_ptr<codesh::ast::type::type_ast_node>> &codesh::ast::method::method_declaration_ast_node::
    get_exceptions_thrown()
{
    return exceptions_thrown;
}