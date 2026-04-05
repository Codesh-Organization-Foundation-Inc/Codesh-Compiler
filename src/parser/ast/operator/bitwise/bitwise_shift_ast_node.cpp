#include "bitwise_shift_ast_node.h"

codesh::ast::op::bitwise_shift_ast_node::bitwise_shift_ast_node(const lexer::code_position code_position,
                                                                std::unique_ptr<value_ast_node> left,
                                                                std::unique_ptr<value_ast_node> right,
                                                                const direction shift_direction) :
    binary_ast_node(code_position, std::move(left), std::move(right)), shift_direction(shift_direction)
{
}

std::string codesh::ast::op::bitwise_shift_ast_node::to_pretty_string() const
{
    return nullptr;
}