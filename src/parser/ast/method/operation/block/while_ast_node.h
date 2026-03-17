#pragma once

#include "parser/ast/impl/i_constant_pool_emitter.h"
#include "parser/ast/method/operation/method_operation_ast_node.h"
#include "parser/ast/var_reference/value_ast_node.h"

#include <memory>

namespace codesh::ast::method
{
class method_scope_ast_node;
}

namespace codesh::ast::block
{

class while_ast_node final : public method::operation::method_operation_ast_node, public impl::i_constant_pool_emitter
{
    std::unique_ptr<var_reference::value_ast_node> condition;
    method::method_scope_ast_node &body_scope;

public:
    while_ast_node(lexer::code_position code_position, std::unique_ptr<var_reference::value_ast_node> condition, method::method_scope_ast_node &body_scope);

    [[nodiscard]] var_reference::value_ast_node &get_condition() const;
    [[nodiscard]] method::method_scope_ast_node &get_body_scope() const;

    void set_statement_index(size_t statement_index) override;

    void emit_constants(const compilation_unit_ast_node &root_node,
                        output::jvm_target::constant_pool &constant_pool) override;

    void emit_ir(output::ir::code_block &containing_block,
                 const semantic_analyzer::symbol_table &symbol_table,
                 const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};

}
