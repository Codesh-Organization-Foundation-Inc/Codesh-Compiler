#pragma once

#include "../../impl/binary_ast_node.h"

namespace codesh::ast::op
{

class greater_equals_operator_ast_node final : public impl::binary_ast_node
{
public:
    greater_equals_operator_ast_node(std::unique_ptr<ir_convertable_ast_node> left, std::unique_ptr<ir_convertable_ast_node> right);

    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
                 const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};

}

