#pragma once

#include "method_declaration_ast_node.h"

namespace codesh::ast::method
{

class constructor_declaration_ast_node : public method_declaration_ast_node
{
public:
    explicit constructor_declaration_ast_node(blasphemy::code_position code_position);
};

}
