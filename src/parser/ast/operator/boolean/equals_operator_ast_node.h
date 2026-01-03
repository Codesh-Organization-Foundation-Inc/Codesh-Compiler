#pragma once

#include "../../impl/binary_ast_node.h"

namespace codesh::ast::op
{

class equals_operator_ast_node final : public impl::binary_ast_node
{
public:
    equals_operator_ast_node(std::unique_ptr<i_ir_emitter> left, std::unique_ptr<i_ir_emitter> right);

    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
                 const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};

}

