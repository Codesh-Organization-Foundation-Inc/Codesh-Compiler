#include "class_declaration_ast_node.h"

codesh::ast::type_decl::class_declaration_ast_node::class_declaration_ast_node(
        const blasphemy::code_position code_position, definition::fully_qualified_name name) :
    type_declaration_ast_node(code_position, std::move(name))
{
}

