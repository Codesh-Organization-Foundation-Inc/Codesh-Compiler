#pragma once

#include "assignment_operator_ast_node.h"

namespace codesh::ast::op::assignment
{
class addition_assignment_operator_ast_node final : public assignment_operator_ast_node
{
public:
    addition_assignment_operator_ast_node(std::unique_ptr<variable_reference_ast_node> left, std::unique_ptr<value_ast_node> right);

    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
                 const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};
}