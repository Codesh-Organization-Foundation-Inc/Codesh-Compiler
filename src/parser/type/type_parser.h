#pragma once

#include <memory>
#include <queue>

namespace codesh
{
class token;
}
namespace codesh::ast::type
{
class type_declaration_ast_node;
class attributes_ast_node;
}


namespace codesh::parser
{

std::unique_ptr<ast::type::attributes_ast_node> parse_attributes(std::queue<std::unique_ptr<token>> &tokens);
std::unique_ptr<ast::type::type_declaration_ast_node> parse_type_declaration(std::queue<std::unique_ptr<token>> &tokens);

}
