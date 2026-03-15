#pragma once

#include "parser/ast/impl/i_ir_emitter.h"
#include "method_operation_ast_node.h"

namespace codesh::ast::method::operation
{

class super_call_ast_node : public method_operation_ast_node
{
//TODO: Add parameters

public:
    using method_operation_ast_node::method_operation_ast_node;

    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
               const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};

}
