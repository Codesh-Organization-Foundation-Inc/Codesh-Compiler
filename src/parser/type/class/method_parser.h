#pragma once

#include "../../ast/type_declaration/attributes_ast_node.h"
#include "../../ast/method/operation/method_call_ast_node.h"

#include <memory>
#include <queue>

namespace codesh::parser
{

void parse_method(std::queue<std::unique_ptr<token>> &tokens);
std::unique_ptr<ast::method::operation::method_call_ast_node> parse_methods_call(
    std::queue<std::unique_ptr<token>> &tokens);
}