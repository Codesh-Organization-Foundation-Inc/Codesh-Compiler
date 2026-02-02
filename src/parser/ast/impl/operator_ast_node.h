#pragma once

#include "parser/ast/var_reference/value_ast_node.h"

namespace codesh::ast::impl
{

class operator_ast_node : public var_reference::value_ast_node
{
public:
    using value_ast_node::value_ast_node;

    [[nodiscard]] virtual bool is_value_valid() const = 0;
    [[nodiscard]] virtual std::string to_pretty_string() const = 0;
};

}
