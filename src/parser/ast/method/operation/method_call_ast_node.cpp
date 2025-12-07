#include "method_call_ast_node.h"

std::optional<std::string> &codesh::ast::method::operation::method_call_ast_node::get_resolved_name()
{
    return resolved_name;
}

std::string codesh::ast::method::operation::method_call_ast_node::get_name() const
{
    return fqcn.join();
}

const std::optional<std::string> &codesh::ast::method::operation::method_call_ast_node::get_resolved_name() const
{
    return resolved_name;
}

codesh::definition::fully_qualified_class_name &codesh::ast::method::operation::method_call_ast_node::get_fqcn()
{
    return fqcn;
}

const codesh::definition::fully_qualified_class_name &codesh::ast::method::operation::method_call_ast_node::get_fqcn()
    const
{
    return fqcn;
}

const std::list<std::unique_ptr<codesh::ast::var_reference::value_ast_node>> &codesh::ast::method::operation::
    method_call_ast_node::get_arguments() const
{
    return arguments;
}

std::list<std::unique_ptr<codesh::ast::var_reference::value_ast_node>> &codesh::ast::method::operation::
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
