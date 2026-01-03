#include "method_operation_ast_node.h"

codesh::ast::method::operation::method_operation_ast_node::method_operation_ast_node() :
    statement_index(-1)
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
