#include "increment_decrement_operator_ast_node.h"

#include "parser/ast/var_reference/evaluable_ast_node.h"

codesh::ast::op::assignment::increment_decrement_operator_ast_node::increment_decrement_operator_ast_node(
        const lexer::code_position code_position,
        std::unique_ptr<var_reference::variable_reference_ast_node> variable) :
    assignment_operator_ast_node(
        code_position,
        std::move(variable),
        var_reference::evaluable_ast_node<int>::make_int(code_position, 1)
    )
{
}
