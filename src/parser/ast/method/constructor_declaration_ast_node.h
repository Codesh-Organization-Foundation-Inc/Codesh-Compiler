#pragma once

#include "method_declaration_ast_node.h"

namespace codesh::ast
{

class constructor_declaration_ast_node : public method_declaration_ast_node
{
public:
    constructor_declaration_ast_node();
};

}
