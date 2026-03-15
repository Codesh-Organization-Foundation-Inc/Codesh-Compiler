#pragma once

#include "parser/ast/method/operation/method_operation_ast_node.h"
#include "parser/ast/type/type_ast_node.h"

namespace codesh::output::ir
{
class code_block;
}

namespace codesh::ast::var_reference
{

class value_ast_node : public method::operation::method_operation_ast_node
{
public:
    using method_operation_ast_node::method_operation_ast_node;

    [[nodiscard]] virtual type::type_ast_node *get_type() const = 0;
};

}
