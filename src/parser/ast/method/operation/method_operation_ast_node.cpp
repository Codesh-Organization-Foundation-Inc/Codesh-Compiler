#include "method_operation_ast_node.h"

#include <stdexcept>

codesh::ast::method::operation::method_operation_ast_node::method_operation_ast_node() :
    statement_index(-1),
    method_scope(nullptr)
{
}

int codesh::ast::method::operation::method_operation_ast_node::get_statement_index() const
{
    return statement_index;
}

void codesh::ast::method::operation::method_operation_ast_node::set_statement_index(const int statement_index)
{
    this->statement_index = statement_index;
}

void codesh::ast::method::operation::method_operation_ast_node::set_method_scope(
    semantic_analyzer::method_scope_symbol *method_scope)
{
    this->method_scope = method_scope;
}

codesh::semantic_analyzer::method_scope_symbol &codesh::ast::method::operation::method_operation_ast_node::
    get_method_scope() const
{
    if (method_scope == nullptr)
    {
        throw std::runtime_error("Attempted to get an operation's method scope, yet one was yet to be resolved");
    }

    return *method_scope;
}
