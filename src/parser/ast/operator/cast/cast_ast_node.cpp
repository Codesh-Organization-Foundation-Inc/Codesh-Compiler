#include "cast_ast_node.h"

codesh::ast::op::assignment::cast_ast_node::cast_ast_node(const lexer::code_position code_position,
        std::unique_ptr<value_ast_node> value) :
    value_ast_node(code_position),
    value(std::move(value))
{
}

codesh::ast::var_reference::value_ast_node &codesh::ast::op::assignment::cast_ast_node::get_value() const
{
    return *value;
}