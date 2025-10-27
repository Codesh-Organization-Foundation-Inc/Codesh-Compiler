#pragma once

#include "../impl/ast_node.h"

#include <string>

namespace codesh::ast::type
{

class type_declaration_ast_node : public impl::ast_node
{
    const std::string name;

public:
    explicit type_declaration_ast_node(std::string name);
};

}
