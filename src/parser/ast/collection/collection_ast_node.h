#pragma once

#include "parser/ast/var_reference/value_ast_node.h"

namespace codesh::ast::collection
{

class collection_ast_node : public var_reference::value_ast_node
{
public:
    using value_ast_node::value_ast_node;
};

}
