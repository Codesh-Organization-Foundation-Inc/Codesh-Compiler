#pragma once

#include "parser/ast/method/operation/method_call_ast_node.h"
#include "parser/ast/operator/assignment/assignment_operator_ast_node.h"
#include "parser/ast/type_declaration/attributes_ast_node.h"
#include "parser/ast/method/operation/block/for_ast_node.h"
#include "parser/ast/method/operation/block/while_ast_node.h"

#include <memory>
#include <queue>

namespace codesh::ast
{
namespace block
{
class if_ast_node;
}
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

enum class var_decl_assignment_policy
{
    ALLOW, // Optionally allows value assignment
    //FIXME: REQUIRE is a result of laziness around making default values.
    //TODO: Add it then remove this
    REQUIRE, // Requires value assignment
    FORBID // Forbids value assignment
};

/**
 * Parses a variable declaration.
 * If the variable was also assigned during initialization, returns its assignment
 * as the 2nd pair.
 */
[[nodiscard]] std::pair<
    std::unique_ptr<ast::local_variable_declaration_ast_node>,
    std::unique_ptr<ast::op::assignment::assignment_operator_ast_node>
> parse_variable_declaration(std::queue<std::unique_ptr<token>> &tokens, var_decl_assignment_policy assignment_policy);


}
