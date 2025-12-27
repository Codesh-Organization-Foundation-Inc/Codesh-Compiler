#pragma once

#include "../../impl/unary_ast_node.h"

namespace codesh::ast::op
{

class not_operator_ast_node final : public impl::unary_ast_node
{
public:
    explicit not_operator_ast_node(std::unique_ptr<i_ir_emitter> child);

    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
                 const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};

}
