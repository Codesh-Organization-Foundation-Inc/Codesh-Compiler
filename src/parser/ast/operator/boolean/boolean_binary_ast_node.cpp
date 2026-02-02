#include "boolean_binary_ast_node.h"

codesh::ast::op::boolean_binary_ast_node::boolean_binary_ast_node(const blasphemy::code_position &code_position,
                                                 std::unique_ptr<value_ast_node> left,
                                                 std::unique_ptr<value_ast_node> right) :
    binary_ast_node(code_position, std::move(left), std::move(right)),
    type(std::make_unique<type::primitive_type_ast_node>(
        code_position,
        definition::primitive_type::BOOLEAN
    ))
{
}

codesh::ast::type::type_ast_node *codesh::ast::op::boolean_binary_ast_node::get_type() const
{
    return type.get();
}
