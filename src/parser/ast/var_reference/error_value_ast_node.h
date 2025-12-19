#pragma once

#include "value_ast_node.h"

namespace codesh::ast::var_reference
{

class error_value_ast_node : public value_ast_node
{
public:
    error_value_ast_node();
};

}
