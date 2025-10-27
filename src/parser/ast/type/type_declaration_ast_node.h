#pragma once

#include "../impl/ast_node.h"

#include <string>

namespace codesh::ast::type
{

class type_declaration_ast_node : public impl::ast_node
{
    std::string name;

public:
    [[nodiscard]] std::string get_name() const;
    void set_name(const std::string &name);
};

}
