#pragma once

#include "../../ast/type_declaration/attributes_ast_node.h"

#include <memory>
#include <queue>

namespace codesh::parser
{

void parse_method(std::queue<std::unique_ptr<token>> &tokens);
void parse_methods_call(std::queue<std::unique_ptr<token>> &tokens);

}