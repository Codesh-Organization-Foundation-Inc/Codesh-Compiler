#pragma once

#include "parser/ast/impl/ast_node.h"

#include "output/jvm_target/class_file_builder.h"
#include "parser/ast/impl/i_constant_pool_emitter.h"
#include "parser/ast/local_variable_declaration_ast_node.h"
#include "parser/ast/method/operation/method_operation_ast_node.h"

#include <deque>
#include <vector>

namespace codesh::ast::method
{

class method_scope_ast_node : public impl::ast_node,
    public impl::i_symbolically_linked<semantic_analyzer::method_scope_symbol>,
    public impl::i_constant_pool_emitter, public impl::i_ir_emitter
{
    method_declaration_ast_node &parent_method;

    std::deque<std::unique_ptr<operation::method_operation_ast_node>> body;
    std::deque<std::unique_ptr<local_variable_declaration_ast_node>> local_variables;

    std::vector<std::unique_ptr<method_scope_ast_node>> method_scopes;

    std::optional<std::reference_wrapper<semantic_analyzer::method_scope_symbol>> scope_symbol;

protected:
    [[nodiscard]] const std::optional<std::reference_wrapper<semantic_analyzer::method_scope_symbol>> &_get_resolved()
        const override;

public:
    method_scope_ast_node(lexer::code_position code_position, method_declaration_ast_node &parent_method);

    void set_resolved(semantic_analyzer::method_scope_symbol &symbol) override;


    [[nodiscard]] method_declaration_ast_node &get_parent_method() const;


    [[nodiscard]] const std::deque<std::unique_ptr<operation::method_operation_ast_node>> &get_body() const;
    void add_statement(std::unique_ptr<operation::method_operation_ast_node> statement);
    void push_front_statement(std::unique_ptr<operation::method_operation_ast_node> statement);

    [[nodiscard]] const std::deque<std::unique_ptr<local_variable_declaration_ast_node>> &get_local_variables()
        const;
    void add_local_variable(std::unique_ptr<local_variable_declaration_ast_node> statement);
    void add_local_variable_front(std::unique_ptr<local_variable_declaration_ast_node> statement);


    method_scope_ast_node &create_method_scope(lexer::code_position code_position);
    [[nodiscard]] const std::vector<std::unique_ptr<method_scope_ast_node>> &get_method_scopes() const;


    /**
     * Marks the end of the scope.
     * All local variables will end at the current position.
     */
    void mark_end() const;


    void emit_constants(const compilation_unit_ast_node &root_node,
                        output::jvm_target::constant_pool &constant_pool) override;

    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
                 const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};

}
