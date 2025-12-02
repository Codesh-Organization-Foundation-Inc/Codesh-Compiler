#include "equals_operator_ast_node.h"

codesh::ast::op::equals_operator_ast_node::equals_operator_ast_node(std::unique_ptr<ir_convertable_ast_node> left,
                                                                           std::unique_ptr<ir_convertable_ast_node> right) :
    binary_ast_node(std::move(left), std::move(right))
{
}

void codesh::ast::op::equals_operator_ast_node::emit_ir(output::ir::code_block &containing_block) const
{
}