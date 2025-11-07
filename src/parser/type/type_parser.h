#pragma once

#include <memory>
#include <queue>

namespace codesh
{
class token;
}
namespace codesh::ast::type_delc
{
class type_declaration_ast_node;
class attributes_ast_node;
}


namespace codesh::parser
{

std::unique_ptr<ast::type_delc::attributes_ast_node> parse_modifiers(std::queue<std::unique_ptr<token>> &tokens);
std::unique_ptr<ast::type_delc::type_declaration_ast_node> parse_type_declaration(std::queue<std::unique_ptr<token>> &tokens);

}
