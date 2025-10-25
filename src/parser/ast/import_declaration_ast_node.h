#pragma once

#include "impl/ast_node.h"

#include <string>

namespace codesh::ast
{

class import_declaration_ast_node final : public impl::ast_node
{
    const std::string package_name;
    const bool is_on_demand;
    const bool is_static;

public:
    import_declaration_ast_node(std::string package_name, bool is_on_demand, bool is_static);

    [[nodiscard]] std::string get_package_name() const;
    [[nodiscard]] bool get_is_on_demand() const;
    [[nodiscard]] bool get_is_static() const;
};

}
