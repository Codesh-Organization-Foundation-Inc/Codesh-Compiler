#include "increment_decrement_operator_ast_node.h"

#include "parser/ast/type/primitive_type_ast_node.h"
#include "parser/ast/var_reference/evaluable_ast_node.h"

codesh::ast::op::assignment::increment_decrement_operator_ast_node::increment_decrement_operator_ast_node(
        const blasphemy::code_position code_position,
        std::unique_ptr<variable_reference_ast_node> variable) :
    assignment_operator_ast_node(
        code_position,
        std::move(variable),
        std::make_unique<var_reference::evaluable_ast_node<int>>(
            code_position,
            std::make_unique<type::primitive_type_ast_node>(
                code_position,
                definition::primitive_type::INTEGER
            ),
            1
        )
    )
{
}
