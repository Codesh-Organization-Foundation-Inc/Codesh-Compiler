#pragma once
#include <memory>
#include <queue>

namespace codesh
{
class token;
}
namespace codesh::ast
{
class import_declaration_ast_node;
}


namespace codesh::parser
{

std::unique_ptr<ast::import_declaration_ast_node> parse_import(std::queue<std::unique_ptr<token>> &tokens);

}
