#pragma once

#include "parser/ast/var_reference/value_ast_node.h"
#include "parser/ast/type/type_ast_node.h"

namespace codesh::ast::op::assignment
{

class cast_ast_node : public var_reference::value_ast_node
{
    std::unique_ptr<value_ast_node> left;
    type::type_ast_node *right;

public:
    cast_ast_node(blasphemy::code_position code_position, std::unique_ptr<value_ast_node> left,
            type::type_ast_node *right);

    [[nodiscard]] const value_ast_node &get_left() const;
    [[nodiscard]] type::type_ast_node *get_right() const;

    [[nodiscard]] type::type_ast_node *get_type() const override;

    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
        const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};

}
