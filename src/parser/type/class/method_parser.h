#pragma once

#include "../../ast/type_declaration/attributes_ast_node.h"
#include "../../ast/method/operation/method_call_ast_node.h"

#include <memory>
#include <queue>

namespace codesh::ast
{
class variable_declaration_ast_node;
}

namespace codesh::ast::method
{
class method_declaration_ast_node;
}

namespace codesh::parser
{

void parse_method(std::queue<std::unique_ptr<token>> &tokens, ast::method::method_declaration_ast_node &method_decl);

[[nodiscard]] std::unique_ptr<ast::method::operation::method_call_ast_node> parse_methods_call(
    std::queue<std::unique_ptr<token>> &tokens);

[[nodiscard]] std::unique_ptr<ast::variable_declaration_ast_node> parse_variable_declaration(
    std::queue<std::unique_ptr<token>> &tokens);

}