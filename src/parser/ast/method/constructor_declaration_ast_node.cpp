#include "constructor_declaration_ast_node.h"

#include "parser/ast/type/primitive_type_ast_node.h"

codesh::ast::method::constructor_declaration_ast_node::constructor_declaration_ast_node(
        const blasphemy::code_position code_position) :
    method_declaration_ast_node(code_position, "<init>")
{
    set_return_type(std::make_unique<type::primitive_type_ast_node>(code_position, definition::primitive_type::VOID));
}
