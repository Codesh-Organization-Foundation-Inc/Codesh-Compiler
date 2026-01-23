#pragma once

#include "defenition/basad_type.h"

#include <memory>
#include <queue>

namespace codesh
{
class token;
}
namespace codesh::ast::type_decl
{
class class_declaration_ast_node;
}


namespace codesh::parser
{

std::unique_ptr<ast::type_decl::class_declaration_ast_node> parse_class_declaration(
        std::queue<std::unique_ptr<token>> &tokens, definition::basad_type basad_type);

}