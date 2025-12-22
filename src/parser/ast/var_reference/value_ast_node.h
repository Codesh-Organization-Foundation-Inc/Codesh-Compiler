#pragma once

#include "../impl/ast_node.h"
#include "../type/type_ast_node.h"

namespace codesh::ast::var_reference
{

class value_ast_node : public impl::ast_node
{
public:
    [[nodiscard]] virtual type::type_ast_node *get_type() const = 0;
};

}
