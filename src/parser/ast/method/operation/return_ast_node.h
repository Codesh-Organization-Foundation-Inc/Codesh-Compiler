#pragma once

#include "parser/ast/impl/i_ir_emitter.h"
#include "parser/ast/method/operation/method_operation_ast_node.h"

namespace codesh::ast::method::operation
{

class return_ast_node : public method_operation_ast_node
{
//TODO: Add return type

public:
    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
                 const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};

}
