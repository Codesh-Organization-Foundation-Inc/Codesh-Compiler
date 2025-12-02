#pragma once

#include "../../impl/ir_convertable_ast_node.h"

namespace codesh::ast::method::operation
{

class return_ast_node : public impl::ir_convertable_ast_node
{
//TODO: Add return type

public:
    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
               const type_decl::class_declaration_ast_node &containing_class_decl) const override;
};

}
