#include "if_ast_node.h"

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

void codesh::ast::block::if_ast_node::emit_ir(output::ir::code_block &containing_block,
        const semantic_analyzer::symbol_table &symbol_table,
        const type_decl::type_declaration_ast_node &containing_type_decl) const
{
}
