#pragma once
#include "assignment_operator_ast_node.h"

namespace codesh::ast::op::assignment
{
class modulo_assignment_operator_ast_node final : public assignment_operator_ast_node
{
public:
    modulo_assignment_operator_ast_node(std::unique_ptr<variable_reference_ast_node> left, std::unique_ptr<value_ast_node> right);

    [[nodiscard]] std::string to_pretty_string() const override;

    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
                 const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};
}
