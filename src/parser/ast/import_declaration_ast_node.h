#pragma once

#include "defenition/fully_qualified_name.h"
#include "parser/ast/impl/ast_node.h"

namespace codesh::ast
{

class import_declaration_ast_node final : public impl::ast_node
{
    definition::fully_qualified_name package_name;
    bool is_static;

public:
    explicit import_declaration_ast_node(lexer::code_position code_position);

    [[nodiscard]] definition::fully_qualified_name &get_package_name();
    [[nodiscard]] const definition::fully_qualified_name &get_package_name() const;
    [[nodiscard]] bool get_is_static() const;

    void set_is_static(bool is_static);
};

}
