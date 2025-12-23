#pragma once

#include "../../defenition/fully_qualified_class_name.h"
#include "impl/ast_node.h"

namespace codesh::ast
{

class import_declaration_ast_node final : public impl::ast_node
{
    definition::fully_qualified_class_name package_name;
    bool is_on_demand;
    bool is_static;

public:
    import_declaration_ast_node();

    [[nodiscard]] definition::fully_qualified_class_name &get_package_name();
    [[nodiscard]] bool get_is_on_demand() const;
    [[nodiscard]] bool get_is_static() const;

    void set_package_name(const definition::fully_qualified_class_name &package_name);
    void set_is_on_demand(bool is_on_demand);
    void set_is_static(bool is_static);
};

}
