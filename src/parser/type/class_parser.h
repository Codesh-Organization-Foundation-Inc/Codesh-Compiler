#pragma once
#include "../ast/type_declaration/attributes_ast_node.h"

#include <memory>
#include <queue>

namespace codesh::ast::type_delc
{
class class_declaration_ast_node;
}


namespace codesh::parser
{

std::unique_ptr<ast::type_delc::class_declaration_ast_node> parse_class_declaration(
        std::queue<std::unique_ptr<token>> &tokens);

}