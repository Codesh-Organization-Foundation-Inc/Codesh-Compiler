#pragma once

#include "parser/ast/impl/binary_ast_node.h"
#include "parser/ast/var_reference/array_access_ast_node.h"

namespace codesh::ast::op::assignment
{

class array_assign_operator_ast_node final : public impl::binary_ast_node
{
public:
    array_assign_operator_ast_node(lexer::code_position code_position,
            std::unique_ptr<array_access_ast_node> left, std::unique_ptr<value_ast_node> right);

    [[nodiscard]] std::string to_pretty_string() const override;

    [[nodiscard]] type::type_ast_node *get_type() const override;

    [[nodiscard]] bool is_value_valid(const semantic_analyzer::semantic_context &context) const override;

    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
                 const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};

}
