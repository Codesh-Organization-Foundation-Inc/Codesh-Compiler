#include "method_scope_ast_node.h"

#include "method_declaration_ast_node.h"
#include "output/ir/code_block.h"
#include "output/ir/instruction/scope_marker.h"

const std::optional<std::reference_wrapper<codesh::semantic_analyzer::method_scope_symbol>> &codesh::ast::method::
    method_scope_ast_node::_get_resolved() const
{
    return scope_symbol;
}

codesh::ast::method::method_scope_ast_node::method_scope_ast_node(const lexer::code_position code_position,
        method_declaration_ast_node &parent_method) :
    ast_node(code_position), parent_method(parent_method)
{
}

void codesh::ast::method::method_scope_ast_node::set_resolved(semantic_analyzer::method_scope_symbol &symbol)
{
    scope_symbol.emplace(symbol);
}

codesh::ast::method::method_declaration_ast_node &codesh::ast::method::method_scope_ast_node::get_parent_method() const
{
    return parent_method;
}

const std::deque<std::unique_ptr<codesh::ast::method::operation::method_operation_ast_node>> &codesh::ast::method::
    method_scope_ast_node::get_body() const
{
    return body;
}

void codesh::ast::method::method_scope_ast_node::add_statement(
        std::unique_ptr<operation::method_operation_ast_node> statement)
{
    statement->set_statement_index(body.size());
    body.emplace_back(std::move(statement));
}

void codesh::ast::method::method_scope_ast_node::push_front_statement(
    std::unique_ptr<operation::method_operation_ast_node> statement)
{
    // Increment the index of each element by 1 (as we are pushing to the front)
    for (const auto &stmnt : body)
    {
        stmnt->set_statement_index(stmnt->get_statement_index() + 1);
    }
    for (const auto &stmnt : local_variables)
    {
        stmnt->set_accessible_from(stmnt->get_accessible_from() + 1);
        stmnt->set_accessible_to(stmnt->get_accessible_to() + 1);
    }

    statement->set_statement_index(0);
    body.emplace_front(std::move(statement));
}

const std::deque<std::unique_ptr<codesh::ast::local_variable_declaration_ast_node>> &codesh::ast::method::
    method_scope_ast_node::get_local_variables() const
{
    return local_variables;
}

void codesh::ast::method::method_scope_ast_node::add_local_variable(
    std::unique_ptr<local_variable_declaration_ast_node> statement)
{
    statement->set_accessible_from(body.size());
    local_variables.emplace_back(std::move(statement));
}

void codesh::ast::method::method_scope_ast_node::add_local_variable_front(
    std::unique_ptr<local_variable_declaration_ast_node> statement)
{
    statement->set_accessible_from(body.size());
    local_variables.emplace_front(std::move(statement));
}

codesh::ast::method::method_scope_ast_node &codesh::ast::method::method_scope_ast_node::
    create_method_scope(const lexer::code_position code_position)
{
    return *method_scopes.emplace_back(
        std::make_unique<method_scope_ast_node>(code_position, parent_method)
    );
}

const std::vector<std::unique_ptr<codesh::ast::method::method_scope_ast_node>> &codesh::ast::method::
    method_scope_ast_node::get_method_scopes() const
{
    return method_scopes;
}

void codesh::ast::method::method_scope_ast_node::mark_end() const
{
    const size_t last_statement_index = body.size() - 1;

    for (const auto &local_var : local_variables)
    {
        local_var->set_accessible_to(last_statement_index);
    }
}

void codesh::ast::method::method_scope_ast_node::emit_constants(const compilation_unit_ast_node &root_node,
                                                                output::jvm_target::constant_pool &constant_pool)
{
    for (const auto &local_var : local_variables)
    {
        local_var->emit_constants(root_node, constant_pool);
    }

    for (const auto &statement : get_body())
    {
        auto *constant_emitter = dynamic_cast<i_constant_pool_emitter *>(statement.get());
        if (!constant_emitter)
            continue;

        constant_emitter->emit_constants(root_node, constant_pool);
    }
}

void codesh::ast::method::method_scope_ast_node::emit_ir(
    output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
    const type_decl::type_declaration_ast_node &containing_type_decl) const
{
    const bool is_inner_scope = &parent_method.get_method_scope() != this;

    if (is_inner_scope)
    {
        containing_block.add_instruction(std::make_unique<output::ir::scope_begin_marker>(*this));
    }

    for (const auto &method_op : get_body())
    {
        const auto ir_emitter = dynamic_cast<i_ir_emitter *>(method_op.get());
        if (!ir_emitter)
            continue;

        ir_emitter->emit_ir(containing_block, symbol_table, containing_type_decl);
    }

    if (is_inner_scope)
    {
        containing_block.add_instruction(std::make_unique<output::ir::scope_end_marker>(*this));
    }
}
