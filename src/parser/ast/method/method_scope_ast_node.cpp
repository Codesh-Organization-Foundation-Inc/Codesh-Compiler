#include "method_scope_ast_node.h"

#include "method_declaration_ast_node.h"
#include "parser/ast/impl/binary_ast_node.h"
#include "parser/ast/impl/unary_ast_node.h"
#include "parser/ast/method/operation/block/for_ast_node.h"
#include "parser/ast/method/operation/block/if_ast_node.h"
#include "parser/ast/method/operation/block/while_ast_node.h"
#include "parser/ast/method/operation/method_call_ast_node.h"
#include "parser/ast/method/operation/return_ast_node.h"
#include "parser/ast/var_reference/value_ast_node.h"

/**
 * Recursively sets all statement indexes for the given statement and its child statements.
 */
static void set_all_statement_indexes(codesh::ast::method::operation::method_operation_ast_node *statement,
        size_t index);


const std::optional<std::reference_wrapper<codesh::semantic_analyzer::method_scope_symbol>> &codesh::ast::method::
    method_scope_ast_node::_get_resolved() const
{
    return scope_symbol;
}

codesh::ast::method::method_scope_ast_node::method_scope_ast_node(method_declaration_ast_node &parent_method) :
    parent_method(parent_method)
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

const std::list<std::unique_ptr<codesh::ast::method::operation::method_operation_ast_node>> &codesh::ast::method::
    method_scope_ast_node::get_body() const
{
    return body;
}

void codesh::ast::method::method_scope_ast_node::add_statement(
        std::unique_ptr<operation::method_operation_ast_node> statement)
{
    set_all_statement_indexes(statement.get(), body.size());
    body.emplace_back(std::move(statement));
}

static void set_all_statement_indexes(codesh::ast::method::operation::method_operation_ast_node *statement,
        const size_t index)
{
    if (statement == nullptr)
        return;

    statement->set_statement_index(index);

    if (const auto *bin_op = dynamic_cast<codesh::ast::impl::binary_ast_node *>(statement))
    {
        set_all_statement_indexes(&bin_op->get_left(), index);
        set_all_statement_indexes(&bin_op->get_right(), index);
    }
    else if (const auto *un_op = dynamic_cast<codesh::ast::impl::unary_ast_node *>(statement))
    {
        set_all_statement_indexes(&un_op->get_child(), index);
    }
    else if (const auto *call_op = dynamic_cast<codesh::ast::method::operation::method_call_ast_node *>(statement))
    {
        for (const auto &arg : call_op->get_arguments())
        {
            set_all_statement_indexes(arg.get(), index);
        }
    }
    else if (const auto *if_op = dynamic_cast<codesh::ast::block::if_ast_node *>(statement))
    {
        set_all_statement_indexes(if_op->get_if_branch().condition.get(), index);
        for (const auto &else_if : if_op->get_else_if_branches())
        {
            set_all_statement_indexes(else_if.condition.get(), index);
        }
    }
    else if (const auto *while_op = dynamic_cast<codesh::ast::block::while_ast_node *>(statement))
    {
        set_all_statement_indexes(&while_op->get_condition(), index);
    }
    else if (const auto *for_op = dynamic_cast<codesh::ast::block::for_ast_node *>(statement))
    {
        set_all_statement_indexes(&for_op->get_collection(), index);
    }
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

const std::list<std::unique_ptr<codesh::ast::local_variable_declaration_ast_node>> &codesh::ast::method::
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

codesh::ast::method::method_scope_ast_node &codesh::ast::method::method_scope_ast_node::
    create_method_scope()
{
    return *method_scopes.emplace_back(std::make_unique<method_scope_ast_node>(parent_method));
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
    for (const auto &method_op : get_body())
    {
        const auto ir_emitter = dynamic_cast<i_ir_emitter *>(method_op.get());
        if (!ir_emitter)
            continue;

        ir_emitter->emit_ir(containing_block, symbol_table, containing_type_decl);
    }
}
