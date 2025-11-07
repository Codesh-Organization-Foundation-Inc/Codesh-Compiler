#pragma once

#include "type_ast_node.h"

#include <string>

namespace codesh::ast::type
{

namespace type_decl
{
class attributes_ast_node;
}


class custom_type_ast_node : public type_ast_node
{
    std::string name;

public:
    [[nodiscard]] std::string get_name() const;
    void set_name(const std::string &name);
};

}
