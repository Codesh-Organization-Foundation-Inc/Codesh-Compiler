#pragma once

#include "../impl/i_ir_emitter.h"
#include "../type/type_ast_node.h"

namespace codesh::output::ir
{
class code_block;
}

namespace codesh::ast::var_reference
{

class value_ast_node : public impl::i_ir_emitter
{
public:
    [[nodiscard]] virtual type::type_ast_node *get_type() const = 0;
};

}
