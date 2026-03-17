#pragma once

#include "assignment_operator_ast_node.h"

namespace codesh::ast::op::assignment
{

class increment_decrement_operator_ast_node : public assignment_operator_ast_node
{
protected:
    increment_decrement_operator_ast_node(lexer::code_position code_position,
            std::unique_ptr<variable_reference_ast_node> variable);
};

}
