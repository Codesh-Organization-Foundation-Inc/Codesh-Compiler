#pragma once
#include "type_declaration_ast_node.h"

class interface_declaration_ast_node final : public codesh::ast::type_decl::type_declaration_ast_node
{

public:
    interface_declaration_ast_node(codesh::lexer::code_position code_position,
            codesh::definition::fully_qualified_name name);
};
