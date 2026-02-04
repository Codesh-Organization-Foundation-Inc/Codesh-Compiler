#pragma once

#include "method_operation_ast_node.h"
#include "parser/ast/impl/i_ir_emitter.h"
#include "parser/ast/var_reference/value_ast_node.h"

namespace codesh::ast::method::operation
{

class return_ast_node : public method_operation_ast_node
{
    std::unique_ptr<var_reference::value_ast_node> return_value;

public:
    using method_operation_ast_node::method_operation_ast_node;

    return_ast_node(
        blasphemy::code_position code_position,
        std::unique_ptr<var_reference::value_ast_node> return_value
    );

    [[nodiscard]] var_reference::value_ast_node *get_return_value() const;

    void set_statement_index(size_t statement_index) override;

    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
                 const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};

}
