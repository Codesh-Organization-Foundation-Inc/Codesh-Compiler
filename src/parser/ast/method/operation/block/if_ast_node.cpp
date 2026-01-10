#include "if_ast_node.h"

codesh::ast::block::if_ast_node::if_ast_node(
    std::unique_ptr<var_reference::value_ast_node> condition,
    method::method_scope_ast_node &if_scope,
    std::optional<std::reference_wrapper<method::method_scope_ast_node>> else_scope
) : condition(std::move(condition)), if_scope(if_scope), else_scope(else_scope)
{
}

const codesh::ast::var_reference::value_ast_node &codesh::ast::block::if_ast_node::get_condition() const
{
    return *condition;
}

codesh::ast::method::method_scope_ast_node &codesh::ast::block::if_ast_node::get_if_scope() const
{
    return if_scope;
}

std::optional<std::reference_wrapper<
    codesh::ast::method::method_scope_ast_node
>> codesh::ast::block::if_ast_node::get_else_scope() const
{
    return else_scope;
}

void codesh::ast::block::if_ast_node::set_condition(
    std::unique_ptr<codesh::ast::var_reference::value_ast_node> new_condition
)
{
    condition = std::move(new_condition);
}

void codesh::ast::block::if_ast_node::clear_else_scope()
{
    else_scope.reset();
}
