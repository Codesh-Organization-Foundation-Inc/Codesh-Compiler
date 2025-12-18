#include "method_call_ast_node.h"

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
