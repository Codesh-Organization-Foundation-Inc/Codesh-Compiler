#pragma once

#include "type_declaration_ast_node.h"

namespace codesh::ast::type
{

class class_declaration_ast_node final : public type_declaration_ast_node
{
    //TODO: Add extends and implements
public:
    explicit class_declaration_ast_node(const std::string &name);
};

}
