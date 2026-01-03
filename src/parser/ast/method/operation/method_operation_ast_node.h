#pragma once

#include "../../impl/ast_node.h"
#include "../../impl/i_ir_emitter.h"

#include <cstddef>

namespace codesh::semantic_analyzer
{
class method_scope_symbol;
}

namespace codesh::ast::method::operation
{

class method_operation_ast_node : impl::ast_node, public impl::i_ir_emitter
{
    size_t statement_index;

public:
    explicit method_operation_ast_node();

    [[nodiscard]] size_t get_statement_index() const;
    virtual void set_statement_index(size_t statement_index);
};

}
