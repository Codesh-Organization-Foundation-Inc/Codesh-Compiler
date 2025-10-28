#pragma once
#include "ast/type/attributes_ast_node.h"

#include <memory>
#include <queue>

namespace codesh::ast::type
{
class class_declaration_ast_node;
}


namespace codesh::parser
{

std::unique_ptr<ast::type::class_declaration_ast_node> parse_class_declaration(
        std::queue<std::unique_ptr<token>> &tokens);
std::unique_ptr<ast::type::attributes_ast_node> parse_attributes(std::queue<std::unique_ptr<token>> &tokens);

}