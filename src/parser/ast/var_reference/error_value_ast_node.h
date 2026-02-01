#pragma once

#include "value_ast_node.h"

namespace codesh::ast::var_reference
{

class error_value_ast_node : public value_ast_node
{
public:
    using value_ast_node::value_ast_node;

    [[nodiscard]] type::type_ast_node *get_type() const override;

    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
                 const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};

}
