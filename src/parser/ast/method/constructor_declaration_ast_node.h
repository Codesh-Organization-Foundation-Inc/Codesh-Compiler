#pragma once

#include "parser/ast/method/method_declaration_ast_node.h"

namespace codesh::ast::method
{

class constructor_declaration_ast_node : public method_declaration_ast_node
{
public:
    constructor_declaration_ast_node();
};

}
