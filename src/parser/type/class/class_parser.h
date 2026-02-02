#pragma once

#include "blasphemy/blasphemy_collector.h"

#include <memory>
#include <queue>

namespace codesh
{
namespace ast
{
class compilation_unit_ast_node;
}
class token;
}
namespace codesh::ast::type_decl
{
class class_declaration_ast_node;
}


namespace codesh::parser
{

std::unique_ptr<ast::type_decl::class_declaration_ast_node> parse_class_declaration(
        blasphemy::code_position code_position, std::queue<std::unique_ptr<token>> &tokens);

}