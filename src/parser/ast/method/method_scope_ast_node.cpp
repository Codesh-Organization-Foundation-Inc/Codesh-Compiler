#include "method_scope_ast_node.h"

const std::list<std::unique_ptr<codesh::ast::method::operation::method_operation_ast_node>> &codesh::ast::method::
    method_scope_ast_node::get_body() const
{
    return body;
}

void codesh::ast::method::method_scope_ast_node::add_statement(
    std::unique_ptr<operation::method_operation_ast_node> statement)
{
    statement->set_statement_index(static_cast<int>(body.size()));
    body.emplace_back(std::move(statement));
}

void codesh::ast::method::method_scope_ast_node::push_front_statement(
    std::unique_ptr<operation::method_operation_ast_node> statement)
{
    statement->set_statement_index(static_cast<int>(body.size()));
    body.emplace_front(std::move(statement));
}

const std::list<std::unique_ptr<codesh::ast::local_variable_declaration_ast_node>> &codesh::ast::method::
    method_scope_ast_node::get_local_variables() const
{
    return local_variables;
}

void codesh::ast::method::method_scope_ast_node::add_local_variable(
    std::unique_ptr<local_variable_declaration_ast_node> statement)
{
    local_variables.emplace_back(std::move(statement));
}

void codesh::ast::method::method_scope_ast_node::mark_end() const
{
    const int last_statement_index = static_cast<int>(body.size()) - 1;

    for (const auto &local_var : local_variables)
    {
        local_var->set_accessible_up_to(last_statement_index);
    }
}

void codesh::ast::method::method_scope_ast_node::emit_constants(const compilation_unit_ast_node &root_node,
                                                                output::jvm_target::constant_pool &constant_pool)
{
    for (const auto &statement : get_body())
    {
        if (auto *constant_emitter = dynamic_cast<i_constant_pool_emitter *>(statement.get()))
        {
            constant_emitter->emit_constants(root_node, constant_pool);
        }
    }
}
