#include "constructor_declaration_ast_node.h"

#include "../type/primitive_type_ast_node.h"

codesh::ast::constructor_declaration_ast_node::constructor_declaration_ast_node()
{
    set_name("<init>");
    set_return_type(std::make_unique<type::primitive_type_ast_node>(definition::primitive_type::VOID));
}
