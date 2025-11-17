#pragma once

#include "../method/constructor_declaration_ast_node.h"
#include "../method/method_declaration_ast_node.h"
#include "../type/custom_type_ast_node.h"
#include "type_declaration_ast_node.h"

namespace codesh::ast::type_decl
{

class class_declaration_ast_node final : public type_declaration_ast_node
{
    //TODO: Add implements
    std::unique_ptr<type::custom_type_ast_node> super_class;

    std::list<std::unique_ptr<method_declaration_ast_node>> methods;
    std::list<std::unique_ptr<constructor_declaration_ast_node>> constructors;

public:
    explicit class_declaration_ast_node(const std::string &name);

    [[nodiscard]] type::custom_type_ast_node *get_super_class() const;
    void set_super_class(std::unique_ptr<type::custom_type_ast_node> super_class);

    [[nodiscard]] std::list<std::unique_ptr<method_declaration_ast_node>> &get_methods();
    [[nodiscard]] const std::list<std::unique_ptr<method_declaration_ast_node>> &get_methods() const;

    [[nodiscard]] std::list<std::unique_ptr<constructor_declaration_ast_node>> &get_constructors();
    [[nodiscard]] const std::list<std::unique_ptr<constructor_declaration_ast_node>> &get_constructors() const;
};

}
