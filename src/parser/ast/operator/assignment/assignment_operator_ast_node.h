#pragma once

#include "output/ir/instruction.h"
#include "parser/ast/impl/binary_ast_node.h"
#include "parser/ast/var_reference/variable_reference_ast_node.h"

#include <optional>

namespace codesh::ast::op::assignment
{

class assignment_operator_ast_node : public impl::binary_ast_node
{
public:
    assignment_operator_ast_node(blasphemy::code_position code_position,
            std::unique_ptr<variable_reference_ast_node> left, std::unique_ptr<value_ast_node> right);

    [[nodiscard]] variable_reference_ast_node &get_left() const override;

    [[nodiscard]] type::type_ast_node *get_type() const override;

    /**
     * @return The operator type for compound assignments (+= etc.),
     * or std::nullopt for plain assignment (=).
     */
    [[nodiscard]] virtual std::optional<output::ir::operator_type> get_operator_type() const = 0;

    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
                 const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};

}
