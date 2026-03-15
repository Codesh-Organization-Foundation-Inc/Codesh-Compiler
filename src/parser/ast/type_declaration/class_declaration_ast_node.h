#pragma once

#include "type_declaration_ast_node.h"

namespace codesh::ast::type_decl
{

class class_declaration_ast_node final : public type_declaration_ast_node
{

public:
    class_declaration_ast_node(blasphemy::code_position code_position, definition::fully_qualified_name name);
};

}
