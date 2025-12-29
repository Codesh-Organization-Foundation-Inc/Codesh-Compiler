#pragma once

#include "../../impl/ast_node.h"
#include "../../impl/i_ir_emitter.h"

namespace codesh::semantic_analyzer
{
class method_scope_symbol;
}

namespace codesh::ast::method::operation
{

class method_operation_ast_node : impl::ast_node, public impl::i_ir_emitter
{
    int statement_index;
    semantic_analyzer::method_scope_symbol *method_scope;

public:
    method_operation_ast_node();

    [[nodiscard]] int get_statement_index() const;
    void set_statement_index(int statement_index);

    void set_method_scope(semantic_analyzer::method_scope_symbol *method_scope);
    [[nodiscard]] semantic_analyzer::method_scope_symbol &get_method_scope() const;
};

}
