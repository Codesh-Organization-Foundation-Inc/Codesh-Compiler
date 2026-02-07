#pragma once

#include "method_declaration_ast_node.h"
#include "parser/ast/type/custom_type_ast_node.h"

namespace codesh::ast::method
{

class constructor_declaration_ast_node : public method_declaration_ast_node
{
    std::unique_ptr<type::custom_type_ast_node> constructed_type;

public:
    constructor_declaration_ast_node(blasphemy::code_position code_position,
            std::unique_ptr<type::custom_type_ast_node> constructed_type);

    [[nodiscard]] type::custom_type_ast_node &get_constructed_type() const;

    [[nodiscard]] std::string to_pretty_string() const override;
};

}
