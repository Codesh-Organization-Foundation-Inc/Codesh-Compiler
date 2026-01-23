#include "class_declaration_ast_node.h"

codesh::ast::type_decl::class_declaration_ast_node::class_declaration_ast_node(
        definition::fully_qualified_name name, const definition::basad_type basad_type)
    : type_declaration_ast_node(std::move(name), basad_type)
{
}

