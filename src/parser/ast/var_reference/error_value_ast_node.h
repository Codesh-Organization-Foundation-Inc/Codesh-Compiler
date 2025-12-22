#pragma once

#include "value_ast_node.h"

namespace codesh::ast::var_reference
{

class error_value_ast_node : public value_ast_node
{
public:
    [[nodiscard]] type::type_ast_node *get_type() const override;
};

}
