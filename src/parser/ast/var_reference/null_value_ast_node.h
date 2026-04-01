#pragma once

#include "parser/ast/type/null_type_ast_node.h"
#include "value_ast_node.h"

namespace codesh::ast::var_reference
{

class null_value_ast_node final : public value_ast_node
{
    const type::null_type_ast_node null_type;

public:
    explicit null_value_ast_node(lexer::code_position code_position);

    [[nodiscard]] type::type_ast_node *get_type() const override;

    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
                 const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};

}
