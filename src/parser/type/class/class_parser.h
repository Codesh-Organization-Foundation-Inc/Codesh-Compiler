#pragma once

#include "lexer/source_file_info.h"
#include "parser/ast/local_variable_declaration_ast_node.h"

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
        lexer::code_position code_position, std::queue<std::unique_ptr<token>> &tokens);

std::vector<std::unique_ptr<ast::local_variable_declaration_ast_node>> parse_parameter_list(
        std::queue<std::unique_ptr<token>> &tokens);

}