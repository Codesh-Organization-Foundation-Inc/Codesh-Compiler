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

const std::list<codesh::ast::block::conditioned_scope_container> &codesh::ast::block::if_ast_node::
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
    //TODO: Add else-if and else
}

void codesh::ast::block::if_ast_node::emit_ir(output::ir::code_block &containing_block,
        const semantic_analyzer::symbol_table &symbol_table,
        const type_decl::type_declaration_ast_node &containing_type_decl) const
{
    const auto &if_cond = *if_branch.condition;

    // Pre-process the if block such that we can determine its size beforehand
    output::ir::code_block if_block;
    if_branch.scope.emit_ir(if_block, symbol_table, containing_type_decl);


    if (const auto &primitive_type = dynamic_cast<const type::primitive_type_ast_node *>(if_cond.get_type()))
    {
        if (primitive_type->get_type() == definition::primitive_type::BOOLEAN)
        {
            if_cond.emit_ir(containing_block, symbol_table, containing_type_decl);

            // If false, jump 'till after the block.
            containing_block.add_instruction(std::make_unique<output::ir::if_instruction>(
                output::ir::if_type::IS_ZERO,
                if_block.size()
            ));
        }
    }


    containing_block.consume_code_block(std::move(if_block));
}
