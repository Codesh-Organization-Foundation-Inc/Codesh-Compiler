#pragma once

#include "parser/ast/method/operation/method_call_ast_node.h"
#include "parser/ast/operator/assignment/assignment_operator_ast_node.h"
#include "parser/ast/type_declaration/attributes_ast_node.h"

#include <memory>
#include <queue>

namespace codesh::ast
{
class local_variable_declaration_ast_node;
}

namespace codesh::ast::method
{
class method_scope_ast_node;
}

namespace codesh::parser
{

void parse_method_scope(std::queue<std::unique_ptr<token>> &tokens, ast::method::method_scope_ast_node &method_scope);

[[nodiscard]] std::unique_ptr<ast::method::operation::method_call_ast_node> parse_methods_call(
    std::queue<std::unique_ptr<token>> &tokens);

/**
 * Parses a variable declaration.
 * If the variable was also assigned during initialization, returns its assignment
 * as the 2nd pair.
 */
[[nodiscard]] std::pair<
    std::unique_ptr<ast::local_variable_declaration_ast_node>,
    std::unique_ptr<ast::op::assignment::assignment_operator_ast_node>
> parse_variable_declaration(std::queue<std::unique_ptr<token>> &tokens);

}