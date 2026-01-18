#include "if_ast_node.h"

#include "../../../../../defenition/primitive_type.h"
#include "../../../../../output/ir/code_block.h"
#include "../../../type/primitive_type_ast_node.h"
#include "../../method_scope_ast_node.h"

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

    std::vector<std::reference_wrapper<output::ir::instruction>> gotos;
    // 1 If + as many else-ifs
    gotos.reserve(1 + else_if_branches.size());


    // If
    emitted_size += emit_branch_ir(
        if_branch,

        !else_if_branches.empty() || else_branch.has_value(),
        emitted_size,
        containing_block,
        symbol_table,
        containing_type_decl
    );

    gotos.emplace_back(*containing_block.get_instructions().back());


    // Else-if
    for (size_t i = 0; i < else_if_branches.size(); ++i)
    {
        const auto &else_if_branch = else_if_branches.at(i);

        emitted_size += emit_branch_ir(
            else_if_branch,
            i < else_if_branches.size() - 1 || else_branch.has_value(),

            emitted_size,
            containing_block,
            symbol_table,
            containing_type_decl
        );

        gotos.emplace_back(*containing_block.get_instructions().back());
    }

    // Else
    if (else_branch.has_value())
    {
        output::ir::code_block else_block;
        else_branch->get().emit_ir(else_block, symbol_table, containing_type_decl);

        emitted_size += else_block.size();

        containing_block.consume_code_block(std::move(else_block));
    }


    // Update all gotos
    for (const auto &instr : gotos)
    {
        if (const auto goto_instr = dynamic_cast<output::ir::goto_instruction *>(&instr.get()))
        {
            goto_instr->set_target(static_cast<int>(emitted_size));
        }
    }
}

size_t codesh::ast::block::if_ast_node::emit_branch_ir(const conditioned_scope_container &branch,
        const bool has_next_branch, const size_t current_position,
        output::ir::code_block &containing_block,
        const semantic_analyzer::symbol_table &symbol_table,
        const type_decl::type_declaration_ast_node &containing_type_decl)
{
    output::ir::code_block temp_block;

    // Pre-process the branch such that we can determine its size before we emit the condition
    output::ir::code_block if_block;
    branch.scope.emit_ir(if_block, symbol_table, containing_type_decl);

    const size_t if_block_size = if_block.size() + (has_next_branch ? 3 : 0);


    output::ir::if_type if_type;
    const auto &cond = *branch.condition;
    if (const auto &primitive_type = dynamic_cast<const type::primitive_type_ast_node *>(cond.get_type()))
    {
        if (primitive_type->get_type() == definition::primitive_type::BOOLEAN)
        {
            cond.emit_ir(temp_block, symbol_table, containing_type_decl);
            if_type = output::ir::if_type::IS_ZERO;
        }
        //TODO: Add more types
    }

    temp_block.add_instruction(std::make_unique<output::ir::if_instruction>(
        if_type,
        // If false, jump 'till after the block.
        if_block_size
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
