#pragma once

#include "../impl/i_ir_emitter.h"
#include "../method/operation/method_operation_ast_node.h"
#include "../type/type_ast_node.h"

namespace codesh::output::ir
{
class code_block;
}

namespace codesh::ast::var_reference
{

class value_ast_node : public method::operation::method_operation_ast_node
{
public:
    [[nodiscard]] virtual type::type_ast_node *get_type() const = 0;
};

}
