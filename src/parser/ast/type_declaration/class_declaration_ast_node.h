#pragma once

#include "../method/constructor_declaration_ast_node.h"
#include "../type/custom_type_ast_node.h"
#include "type_declaration_ast_node.h"

namespace codesh::ast::type_decl
{

class class_declaration_ast_node final : public type_declaration_ast_node
{
    //TODO: Add implements

    std::list<std::unique_ptr<method::method_declaration_ast_node>> methods;

public:
    explicit class_declaration_ast_node(const std::string &name);

    [[nodiscard]] std::list<std::unique_ptr<method::method_declaration_ast_node>> &get_methods();
    [[nodiscard]] const std::list<std::unique_ptr<method::method_declaration_ast_node>> &get_methods() const;
};

}
