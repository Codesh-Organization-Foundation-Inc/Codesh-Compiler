#include "value_ast_node.h"

codesh::ast::var_reference::value_ast_node::value_ast_node(std::unique_ptr<type::type_ast_node> type) :
    type(std::move(type))
{
}

const codesh::ast::type::type_ast_node *codesh::ast::var_reference::value_ast_node::get_type() const
{
    return type.get();
}
