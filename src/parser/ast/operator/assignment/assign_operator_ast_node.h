#pragma once

#include "parser/ast/var_reference/variable_reference_ast_node.h"
#include "assignment_operator_ast_node.h"

namespace codesh::ast::op::assignment
{

class assign_operator_ast_node final : public assignment_operator_ast_node, public impl::i_ir_emitter
{
public:
    assign_operator_ast_node(blasphemy::code_position code_position, std::unique_ptr<variable_reference_ast_node> left,
            std::unique_ptr<value_ast_node> right);

    [[nodiscard]] std::string to_pretty_string() const override;

    [[nodiscard]] std::optional<output::ir::operator_type> get_operator_type() const override;

    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
                 const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};

}
