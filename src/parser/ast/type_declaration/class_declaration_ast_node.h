#pragma once

#include "type_declaration_ast_node.h"

namespace codesh::ast::type_decl
{

class class_declaration_ast_node final : public type_declaration_ast_node
{

public:
    explicit class_declaration_ast_node(definition::fully_qualified_class_name name);
};

}
