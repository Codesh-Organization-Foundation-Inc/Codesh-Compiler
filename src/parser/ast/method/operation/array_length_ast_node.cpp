#include "array_length_ast_node.h"

codesh::ast::method::operation::array_length_ast_node::array_length_ast_node(const lexer::code_position code_position,
        std::unique_ptr<value_ast_node> child) :
    unary_ast_node(code_position, std::move(child))
{
}

std::string codesh::ast::method::operation::array_length_ast_node::to_pretty_string() const
{
    return "אֹרֶךְ־מַעֲרָךְ";
}
