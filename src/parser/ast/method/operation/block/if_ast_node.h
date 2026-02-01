#pragma once

#include "parser/ast/impl/i_constant_pool_emitter.h"
#include "parser/ast/method/operation/method_operation_ast_node.h"
#include "parser/ast/var_reference/value_ast_node.h"

#include <vector>
#include <memory>
#include <optional>

namespace codesh::output::ir
{
class goto_instruction;
}
namespace codesh::ast::method
{
class method_scope_ast_node;
}


namespace codesh::ast::block // TODO: Additionally nest inside logic namespace
{
/**
 * A container for scopes that are bound to a condition.
 *
 * Matches for if and else-if blocks.
 */
struct conditioned_scope_container
{
    std::unique_ptr<var_reference::value_ast_node> condition;
    method::method_scope_ast_node &scope;
};

class if_ast_node : public method::operation::method_operation_ast_node, public impl::i_constant_pool_emitter
{
    const conditioned_scope_container if_branch;
    std::vector<conditioned_scope_container> else_if_branches;
    std::optional<std::reference_wrapper<method::method_scope_ast_node>> else_branch;

    /**
     * Emits IR for the given branch.
     *
     * If it has a next branch, will append a @link goto_instruction \endlink to be filled later.
     *
     * @returns The size of the branch in bytes
     */
    static size_t emit_branch_ir(const conditioned_scope_container &branch, bool has_next_branch, size_t current_position,
            output::ir::code_block &containing_block,
            const semantic_analyzer::symbol_table &symbol_table,
            const type_decl::type_declaration_ast_node &containing_type_decl) ;

public:
    if_ast_node(blasphemy::code_position code_position, conditioned_scope_container if_branch);


    [[nodiscard]] const conditioned_scope_container &get_if_branch() const;

    void add_else_if_branch(conditioned_scope_container branch);
    [[nodiscard]] const std::vector<conditioned_scope_container> &get_else_if_branches() const;

    [[nodiscard]] std::optional<std::reference_wrapper<method::method_scope_ast_node>> get_else_branch() const;
    void set_else_branch(method::method_scope_ast_node& else_scope);


    void emit_constants(const compilation_unit_ast_node &root_node,
                        output::jvm_target::constant_pool &constant_pool) override;

    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
                 const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};
}
