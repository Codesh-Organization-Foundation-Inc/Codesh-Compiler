#pragma once

#include "../../impl/ast_node.h"
#include "../../impl/i_ir_emitter.h"

namespace codesh::ast::method::operation
{

class method_operation_ast_node : public impl::ast_node, public impl::i_ir_emitter
{
    int statement_index;

public:
    method_operation_ast_node();

    [[nodiscard]] int get_statement_index() const;
    void set_statement_index(int statement_index);
};

}
