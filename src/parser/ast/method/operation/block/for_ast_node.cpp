#include "for_ast_node.h"

using namespace codesh::ast::block;

for_ast_node::for_ast_node(
        std::optional<std::unique_ptr<op::assignment::assign_operator_ast_node>> init,
        std::optional<std::unique_ptr<var_reference::value_ast_node>> condition,
        std::optional<std::unique_ptr<impl::binary_ast_node>> iteration,
        method::method_scope_ast_node &body_scope) :
    init(std::move(init)),
    condition(std::move(condition)),
    iteration(std::move(iteration)),
    body_scope(body_scope)
{
}

const std::optional<std::unique_ptr<codesh::ast::op::assignment::assign_operator_ast_node>> &for_ast_node::get_init() const
{
    return init;
}

const std::optional<std::unique_ptr<codesh::ast::var_reference::value_ast_node>> &for_ast_node::get_condition() const
{
    return condition;
}

const std::optional<std::unique_ptr<codesh::ast::impl::binary_ast_node>> &for_ast_node::get_iteration() const
{
    return iteration;
}

const codesh::ast::method::method_scope_ast_node &for_ast_node::get_body_scope() const
{
    return body_scope;
}

void for_ast_node::emit_ir(
    output::ir::code_block &containing_block,
    const semantic_analyzer::symbol_table &symbol_table,
    const type_decl::type_declaration_ast_node &containing_type_decl
) const
{
}
