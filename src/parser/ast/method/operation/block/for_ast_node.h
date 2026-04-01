#pragma once

#include "parser/ast/operator/assignment/assign_operator_ast_node.h"
#include "parser/ast/var_reference/value_ast_node.h"
#include "parser/ast/method/operation/method_operation_ast_node.h"

#include <memory>

namespace codesh::ast::method
{
class method_scope_ast_node;
}

namespace codesh::ast::block
{

class for_ast_node final : public method::operation::method_operation_ast_node, public impl::i_constant_pool_emitter
{
    method::method_scope_ast_node &body_scope;

    local_variable_declaration_ast_node &iterator;
    std::unique_ptr<var_reference::value_ast_node> collection;

    std::optional<int> skip_constant_cpi;

public:
    for_ast_node(lexer::code_position code_position, std::unique_ptr<var_reference::value_ast_node> collection,
            method::method_scope_ast_node &body_scope);

    [[nodiscard]] local_variable_declaration_ast_node &get_iterator() const;
    [[nodiscard]] var_reference::value_ast_node &get_collection() const;
    [[nodiscard]] method::method_scope_ast_node &get_body_scope() const;
    /**
     * @returns The first inner scope of the for loop, which always only keeps the iterator declaration.
     */
    [[nodiscard]] method::method_scope_ast_node &get_iterator_declaration_scope() const;

    void set_statement_index(size_t statement_index) override;

    void emit_constants(const compilation_unit_ast_node &root_node,
                        output::jvm_target::constant_pool &constant_pool) override;

    void emit_ir(output::ir::code_block &containing_block,
                 const semantic_analyzer::symbol_table &symbol_table,
                 const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};

}
