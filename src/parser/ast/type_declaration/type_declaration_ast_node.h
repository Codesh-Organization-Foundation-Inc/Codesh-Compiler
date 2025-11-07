#pragma once

#include "../impl/ast_node.h"
#include "attributes_ast_node.h"

#include <memory>
#include <string>

namespace codesh::ast::type_delc
{

class type_declaration_ast_node : public impl::ast_node
{
    const std::string name;
    std::unique_ptr<attributes_ast_node> attributes;

public:
    explicit type_declaration_ast_node(std::string name);

    [[nodiscard]] std::string get_name() const;
    [[nodiscard]] attributes_ast_node *get_attributes() const;
    void set_attributes(std::unique_ptr<attributes_ast_node> attributes);
};

}
