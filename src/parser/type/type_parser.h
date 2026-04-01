#pragma once

#include "lexer/source_file_info.h"

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
class type_declaration_ast_node;
class attributes_ast_node;
class variable_declaration_ast_node;
}
namespace codesh::ast::op::assignment
{
class assign_operator_ast_node;
}


namespace codesh::parser
{

enum class var_decl_assignment_policy
{
    ALLOW,   // Optionally allows value assignment
    //FIXME: REQUIRE is a result of laziness around making default values.
    //TODO: Add it then remove this
    REQUIRE, // Requires value assignment
    FORBID   // Forbids value assignment
};

[[nodiscard]] std::unique_ptr<ast::type_decl::attributes_ast_node> parse_modifiers(
        lexer::code_position code_position, std::queue<std::unique_ptr<token>> &tokens);
[[nodiscard]] std::unique_ptr<ast::type_decl::type_declaration_ast_node> parse_type_declaration(
        std::queue<std::unique_ptr<token>> &tokens);

[[nodiscard]] std::unique_ptr<ast::op::assignment::assign_operator_ast_node> parse_variable_declaration(
        ast::type_decl::variable_declaration_ast_node &dest,
        lexer::code_position pos,
        std::queue<std::unique_ptr<token>> &tokens,
        var_decl_assignment_policy policy = var_decl_assignment_policy::REQUIRE);

}
