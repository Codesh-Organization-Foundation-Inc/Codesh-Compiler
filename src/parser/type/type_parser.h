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
class type_declaration_ast_node;
class attributes_ast_node;
}


namespace codesh::parser
{

[[nodiscard]] std::unique_ptr<ast::type_decl::attributes_ast_node> parse_modifiers(std::queue<std::unique_ptr<token>> &tokens);
[[nodiscard]] std::unique_ptr<ast::type_decl::type_declaration_ast_node> parse_type_declaration(
        std::queue<std::unique_ptr<token>> &tokens, definition::basad_type basad_type);

}
