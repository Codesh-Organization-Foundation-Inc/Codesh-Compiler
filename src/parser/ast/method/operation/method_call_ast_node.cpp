#include "method_call_ast_node.h"

#include "../../../../semantic_analyzer/symbol_table/symbol.h"
#include "../util.h"
#include "fmt/xchar.h"

std::optional<codesh::definition::fully_qualified_class_name> &codesh::ast::method::operation::method_call_ast_node::
    _get_resolved_name()
{
    return resolved_name;
}

const codesh::definition::fully_qualified_class_name &codesh::ast::method::operation::method_call_ast_node::get_name()
    const
{
    return name;
}

const std::optional<codesh::definition::fully_qualified_class_name> &codesh::ast::method::operation::
    method_call_ast_node::_get_resolved_name() const
{
    return resolved_name;
}

codesh::definition::fully_qualified_class_name &codesh::ast::method::operation::method_call_ast_node::get_fqcn()
{
    return name;
}

const codesh::definition::fully_qualified_class_name &codesh::ast::method::operation::method_call_ast_node::get_fqcn()
    const
{
    return name;
}

codesh::semantic_analyzer::method_symbol &codesh::ast::method::operation::
    method_call_ast_node::get_referred_method() const
{
    if (!referred_method.has_value())
    {
        throw std::runtime_error("Attempted to get unresolved method call");
    }

    return referred_method.value();
}

void codesh::ast::method::operation::method_call_ast_node::set_referred_method(semantic_analyzer::method_symbol &method)
{
    referred_method.emplace(method);
}

std::string codesh::ast::method::operation::method_call_ast_node::generate_descriptor(const bool resolved) const
{
    if (!resolved)
    {
        throw std::runtime_error("Unable to generate unresolved descriptors for method calls");
    }


    const auto &method = get_referred_method();

    std::vector<std::reference_wrapper<type::type_ast_node>> param_types;
    for (const auto &param : method.get_parameter_types())
    {
        param_types.push_back(*param.get());
    }

    return util::generate_descriptor(true, method.get_return_type(), param_types, method.get_attributes());
}

const std::vector<std::unique_ptr<codesh::ast::var_reference::value_ast_node>> &codesh::ast::method::operation::
    method_call_ast_node::get_arguments() const
{
    return arguments;
}

std::vector<std::unique_ptr<codesh::ast::var_reference::value_ast_node>> &codesh::ast::method::operation::
    method_call_ast_node::get_arguments()
{
    return arguments;
}

void codesh::ast::method::operation::method_call_ast_node::emit_ir(
    output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
    const type_decl::type_declaration_ast_node &containing_type_decl) const
{
    //TODO
}
