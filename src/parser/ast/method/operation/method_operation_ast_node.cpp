#include "method_operation_ast_node.h"

codesh::ast::method::operation::method_operation_ast_node::method_operation_ast_node(
        const blasphemy::code_position code_position) :
    ast_node(code_position), statement_index(-1)
{
}

size_t codesh::ast::method::operation::method_operation_ast_node::get_statement_index() const
{
    return statement_index;
}

void codesh::ast::method::operation::method_operation_ast_node::set_statement_index(const size_t statement_index)
{
    this->statement_index = statement_index;
}
