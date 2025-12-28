#pragma once

#include "../../impl/unary_ast_node.h"

namespace codesh::ast::op
{

class minus_operator_ast_node final : public impl::unary_ast_node
{
public:
    minus_operator_ast_node(std::unique_ptr<ir_emitting_ast_node> child);

    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
                 const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};

}

