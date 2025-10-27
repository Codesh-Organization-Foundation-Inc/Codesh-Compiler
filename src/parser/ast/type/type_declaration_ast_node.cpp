#include "type_declaration_ast_node.h"

#include <utility>

codesh::ast::type::type_declaration_ast_node::type_declaration_ast_node(std::string name) : name(std::move(name))
{
}
