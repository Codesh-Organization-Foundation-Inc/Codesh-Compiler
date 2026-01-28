#include "if_ast_node.h"

#include "output/ir/code_block.h"
#include "parser/ast/operator/boolean/and_operator_ast_node.h"
#include "parser/ast/operator/boolean/or_operator_ast_node.h"
#include "parser/ast/method/method_declaration_ast_node.h"
#include "parser/ast/method/method_scope_ast_node.h"

static codesh::output::ir::code_block build_condition_block(
        const codesh::ast::var_reference::value_ast_node &condition,
        size_t if_block_size,
        const codesh::semantic_analyzer::symbol_table &symbol_table,
        const codesh::ast::type_decl::type_declaration_ast_node &containing_type_decl,
        codesh::output::ir::if_type if_type);

codesh::ast::block::if_ast_node::if_ast_node(conditioned_scope_container if_branch) : if_branch(std::move(if_branch))
{
}

const codesh::ast::block::conditioned_scope_container &codesh::ast::block::if_ast_node::get_if_branch() const
{
    return if_branch;
}

void codesh::ast::block::if_ast_node::add_else_if_branch(conditioned_scope_container branch)
{
    this->else_if_branches.emplace_back(std::move(branch));
}

const std::vector<codesh::ast::block::conditioned_scope_container> &codesh::ast::block::if_ast_node::
    get_else_if_branches() const
{
    return this->else_if_branches;
}

std::optional<std::reference_wrapper<codesh::ast::method::method_scope_ast_node>> codesh::ast::block::if_ast_node::
    get_else_branch() const
{
    return this->else_branch;
}

void codesh::ast::block::if_ast_node::set_else_branch(method::method_scope_ast_node &else_scope)
{
    this->else_branch = else_scope;
}

void codesh::ast::block::if_ast_node::emit_constants(const compilation_unit_ast_node &root_node,
                                                     output::jvm_target::constant_pool &constant_pool)
{
    if_branch.scope.emit_constants(root_node, constant_pool);

    for (const auto &else_if_branch : else_if_branches)
    {
        else_if_branch.scope.emit_constants(root_node, constant_pool);
    }

    if (else_branch.has_value())
    {
        else_branch->get().emit_constants(root_node, constant_pool);
    }
}

void codesh::ast::block::if_ast_node::emit_ir(output::ir::code_block &containing_block,
        const semantic_analyzer::symbol_table &symbol_table,
        const type_decl::type_declaration_ast_node &containing_type_decl) const
{
    size_t emitted_size = 0;
    bool has_next_branch = false;

    std::vector<std::reference_wrapper<output::ir::instruction>> gotos;
    // 1 If + as many else-ifs
    gotos.reserve(1 + else_if_branches.size());


    // If
    has_next_branch = !else_if_branches.empty() || else_branch.has_value();

    emitted_size += emit_branch_ir(
        if_branch,

        has_next_branch,
        emitted_size,
        containing_block,
        symbol_table,
        containing_type_decl
    );

    if (has_next_branch)
    {
        gotos.emplace_back(*containing_block.get_instructions().back());
    }


    // Else-if
    for (size_t i = 0; i < else_if_branches.size(); ++i)
    {
        const auto &else_if_branch = else_if_branches.at(i);

        has_next_branch = i < else_if_branches.size() - 1 || else_branch.has_value();

        emitted_size += emit_branch_ir(
            else_if_branch,

            has_next_branch,
            emitted_size,
            containing_block,
            symbol_table,
            containing_type_decl
        );

        if (has_next_branch)
        {
            gotos.emplace_back(*containing_block.get_instructions().back());
        }
    }

    // Else
    if (else_branch.has_value())
    {
        output::ir::code_block else_block;
        else_branch->get().emit_ir(else_block, symbol_table, containing_type_decl);

        emitted_size += else_block.size();

        containing_block.consume_code_block(std::move(else_block));
    }


    // Update all closure gotos
    for (const auto &instr : gotos)
    {
        dynamic_cast<output::ir::goto_instruction &>(instr.get()).set_target(static_cast<int>(emitted_size));
    }
}

size_t codesh::ast::block::if_ast_node::emit_branch_ir(const conditioned_scope_container &branch,
        const bool has_next_branch, const size_t current_position,
        output::ir::code_block &containing_block,
        const semantic_analyzer::symbol_table &symbol_table,
        const type_decl::type_declaration_ast_node &containing_type_decl)
{
    // Pre-process the branch such that we can determine its size before we emit it
    output::ir::code_block if_block;
    branch.scope.emit_ir(if_block, symbol_table, containing_type_decl);

    const size_t if_block_size = if_block.size() + (has_next_branch ? 3 : 0);


    output::ir::code_block temp_block;
    const auto &cond = *branch.condition;

    temp_block.consume_code_block(build_condition_block(
        cond,
        if_block_size,
        symbol_table,
        containing_type_decl,
        output::ir::if_type::IS_ZERO // Jump on false to skip the if body
    ));

    temp_block.consume_code_block(std::move(if_block));


    if (has_next_branch)
    {
        // If there exists a next branch, then we must skip over it (and every other branch thereof).
        //
        // Since we don't yet know where we will skip, we will leave the current position of the bytecode such that
        // we can later calculate `jump = target - current`.
        temp_block.add_instruction(
            std::make_unique<output::ir::goto_instruction>(current_position + temp_block.size() + 3)
        );
    }


    const size_t emitted_size = temp_block.size();
    containing_block.consume_code_block(std::move(temp_block));
    return emitted_size;
}

static codesh::output::ir::code_block build_condition_block(
        const codesh::ast::var_reference::value_ast_node &condition,
        const size_t if_block_size,
        const codesh::semantic_analyzer::symbol_table &symbol_table,
        const codesh::ast::type_decl::type_declaration_ast_node &containing_type_decl,
        const codesh::output::ir::if_type if_type)
{
    codesh::output::ir::code_block condition_block;

    // Handle AND
    if (const auto and_cond = dynamic_cast<const codesh::ast::op::and_operator_ast_node *>(&condition))
    {
        auto right_block = build_condition_block(
            and_cond->get_right(),
            if_block_size,
            symbol_table,
            containing_type_decl,
            if_type
        );

        size_t jump_size;
        if (if_type == codesh::output::ir::if_type::IS_NONZERO)
        {
            // When jumping on true, short-circuit false by skipping the right block
            jump_size = right_block.size();
        }
        else
        {
            // When jumping on false, short-circuit false by skipping the right block and body
            jump_size = if_block_size + right_block.size();
        }

        auto left_block = build_condition_block(
            and_cond->get_left(),
            jump_size,
            symbol_table,
            containing_type_decl,
            codesh::output::ir::if_type::IS_ZERO
        );

        condition_block.consume_code_block(std::move(left_block));
        condition_block.consume_code_block(std::move(right_block));
        return condition_block;
    }
    // Handle OR
    if (const auto or_cond = dynamic_cast<const codesh::ast::op::or_operator_ast_node *>(&condition))
    {
        auto right_block = build_condition_block(
            or_cond->get_right(),
            if_block_size,
            symbol_table,
            containing_type_decl,
            if_type
        );

        size_t jump_size;
        if (if_type == codesh::output::ir::if_type::IS_ZERO)
        {
            // When jumping on false, short-circuit true by skipping over the right block
            jump_size = right_block.size();
        }
        else
        {
            // When jumping on true, short-circuit true by jumping immediately to the expression
            jump_size = if_block_size;
        }

        auto left_block = build_condition_block(
            or_cond->get_left(),
            jump_size,
            symbol_table,
            containing_type_decl,
            codesh::output::ir::if_type::IS_NONZERO
        );

        condition_block.consume_code_block(std::move(left_block));
        condition_block.consume_code_block(std::move(right_block));
        return condition_block;
    }


    //TODO: This should only be emitted if no other if options exists (see next TODO)
    condition.emit_ir(condition_block, symbol_table, containing_type_decl);
    condition_block.add_instruction(std::make_unique<codesh::output::ir::if_instruction>(
        if_type,
        static_cast<int>(if_block_size)
    ));

    //TODO: Add more if types (bytecode optimizations)
    // if (const auto &primitive_type = dynamic_cast<const type::primitive_type_ast_node *>(cond.get_type()))
    // {
    //     if (primitive_type->get_type() == definition::primitive_type::BOOLEAN)
    //     {
    //     }
    // }

    return condition_block;
}
