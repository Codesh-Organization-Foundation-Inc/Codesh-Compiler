#pragma once

#include "parser/ast/impl/binary_ast_node.h"
#include "parser/ast/var_reference/variable_reference_ast_node.h"

namespace codesh::ast::op::assignment
{

class assignment_operator_ast_node : public impl::binary_ast_node
{
    std::optional<int> rhs_cpi;

public:
    assignment_operator_ast_node(lexer::code_position code_position,
            std::unique_ptr<variable_reference_ast_node> left, std::unique_ptr<value_ast_node> right);

    [[nodiscard]] variable_reference_ast_node &get_left() const override;

    [[nodiscard]] type::type_ast_node *get_type() const override;

    [[nodiscard]] bool is_value_valid() const override;

    [[nodiscard]] virtual output::ir::operator_type get_operator_type() const = 0;


    void emit_constants(const compilation_unit_ast_node &root_node, output::jvm_target::constant_pool &constant_pool)
        override;

    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
                 const type_decl::type_declaration_ast_node &containing_type_decl) const override;

    void emit_field_assignment(const semantic_analyzer::field_symbol &field, output::ir::code_block &containing_block,
            const semantic_analyzer::symbol_table &symbol_table,
            const type_decl::type_declaration_ast_node &containing_type_decl) const;
    void emit_local_var_assignment(const semantic_analyzer::local_variable_symbol &local_var,
            output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
            const type_decl::type_declaration_ast_node &containing_type_decl) const;
};

}
