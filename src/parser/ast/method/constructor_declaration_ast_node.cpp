#include "constructor_declaration_ast_node.h"

#include "parser/ast/type/primitive_type_ast_node.h"

codesh::ast::method::constructor_declaration_ast_node::constructor_declaration_ast_node() :
    method_declaration_ast_node("<init>")
{
    set_return_type(std::make_unique<type::primitive_type_ast_node>(definition::primitive_type::VOID));
}
