#pragma once

#include "impl/ast_node.h"

#include <list>
#include <string>

namespace codesh::ast
{

class import_declaration_ast_node final : public impl::ast_node
{
    std::list<std::string> package_name;
    bool is_on_demand;
    bool is_static;

public:
    import_declaration_ast_node();

    [[nodiscard]] std::list<std::string> &get_package_name();
    [[nodiscard]] bool get_is_on_demand() const;
    [[nodiscard]] bool get_is_static() const;

    void set_package_name(const std::list<std::string> &package_name);
    void set_is_on_demand(bool is_on_demand);
    void set_is_static(bool is_static);
};

}
