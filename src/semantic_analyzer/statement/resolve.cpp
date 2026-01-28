#include "resolve.h"
#include "method_call/resolve.h"
#include "variable_reference/resolve.h"

#include "../../blasphemy/blasphemy_collector.h"
#include "../../blasphemy/details.h"
#include "../../parser/ast/impl/binary_ast_node.h"
#include "../../parser/ast/impl/unary_ast_node.h"
#include "../../parser/ast/method/operation/block/if_ast_node.h"
#include "../../parser/ast/method/operation/block/while_ast_node.h"
#include "../../parser/ast/type/primitive_type_ast_node.h"
#include "../../parser/ast/var_reference/variable_reference_ast_node.h"
#include "../../semantic_analyzer/symbol_table/symbol.h"

static bool resolve_value(const codesh::semantic_analyzer::semantic_context &context,
                               codesh::ast::var_reference::value_ast_node &val_node,
                               const codesh::semantic_analyzer::method_symbol &containing_method,
                               const codesh::semantic_analyzer::method_scope_symbol &scope);

static bool resolve_scope(const codesh::semantic_analyzer::semantic_context &context,
                          const codesh::semantic_analyzer::method_symbol &containing_method,
                          const codesh::ast::method::method_scope_ast_node &scope_node);

static bool resolve_conditioned_scope(const codesh::semantic_analyzer::semantic_context &context,
                          const codesh::semantic_analyzer::method_symbol &containing_method,
                          const codesh::semantic_analyzer::method_scope_symbol &scope,
                          const codesh::ast::block::conditioned_scope_container &conditioned_scope);

static bool is_primitive_type(const codesh::ast::var_reference::value_ast_node &val_node,
                              codesh::definition::primitive_type type,
                              const std::string &blasphemy_details);

static bool is_condition_boolean(const codesh::ast::var_reference::value_ast_node &val_node);


bool codesh::semantic_analyzer::statement::resolve(const semantic_context &context,
                                                   ast::method::operation::method_operation_ast_node &stmnt,
                                                   const method_symbol &containing_method,
                                                   const method_scope_symbol &scope)
{
    if (const auto method_call = dynamic_cast<ast::method::operation::method_call_ast_node *>(&stmnt))
    {
        return method_call::resolve(context, *method_call, containing_method, scope);
    }

    if (const auto var_ref = dynamic_cast<variable_reference_ast_node *>(&stmnt))
    {
        return variable_reference::resolve(context, *var_ref, scope);
    }


    if (const auto if_node = dynamic_cast<ast::block::if_ast_node *>(&stmnt))
    {
        bool all_succeed = true;

        // If
        all_succeed &= resolve_conditioned_scope(context, containing_method, scope, if_node->get_if_branch());

        // Else-if
        for (const auto &else_if_branch : if_node->get_else_if_branches())
        {
            all_succeed &= resolve_conditioned_scope(context, containing_method, scope, else_if_branch);
        }

        // Else
        if (if_node->get_else_branch().has_value())
        {
            all_succeed &= resolve_scope(context, containing_method, if_node->get_else_branch()->get());
        }

        return all_succeed;
    }


    if (const auto while_node = dynamic_cast<ast::block::while_ast_node *>(&stmnt))
    {
        bool all_succeed = true;
        all_succeed &= resolve_value(context, while_node->get_condition(), containing_method, scope);
        all_succeed &= resolve_scope(context, containing_method, while_node->get_body_scope());
        all_succeed &= is_condition_boolean(while_node->get_condition());
        return all_succeed;
    }


    if (const auto unary_op = dynamic_cast<ast::impl::unary_ast_node *>(&stmnt))
    {
        return resolve_value(context, unary_op->get_child(), containing_method, scope);
    }

    if (const auto binary_op = dynamic_cast<ast::impl::binary_ast_node *>(&stmnt))
    {
        bool all_succeed = true;
        all_succeed &= resolve_value(context, binary_op->get_left(), containing_method, scope);
        all_succeed &= resolve_value(context, binary_op->get_right(), containing_method, scope);
        return all_succeed;
    }

    // Probably doesn't need to be resolved.
    return true;
}


static bool resolve_value(const codesh::semantic_analyzer::semantic_context &context,
                          codesh::ast::var_reference::value_ast_node &val_node,
                          const codesh::semantic_analyzer::method_symbol &containing_method,
                          const codesh::semantic_analyzer::method_scope_symbol &scope)
{
    if (const auto var_ref = dynamic_cast<variable_reference_ast_node *>(&val_node))
    {
        return codesh::semantic_analyzer::statement::variable_reference::resolve(context, *var_ref, scope);
    }

    return codesh::semantic_analyzer::statement::resolve(context, val_node, containing_method, scope);
}

static bool resolve_scope(const codesh::semantic_analyzer::semantic_context &context,
                          const codesh::semantic_analyzer::method_symbol &containing_method,
                          const codesh::ast::method::method_scope_ast_node &scope_node)
{
    bool all_succeed = true;

    for (const auto &statement : scope_node.get_body())
    {
        all_succeed &= codesh::semantic_analyzer::statement::resolve(
            context,
            *statement,
            containing_method,
            scope_node.get_resolved()
        );
    }

    return all_succeed;
}

static bool resolve_conditioned_scope(const codesh::semantic_analyzer::semantic_context &context,
                          const codesh::semantic_analyzer::method_symbol &containing_method,
                          const codesh::semantic_analyzer::method_scope_symbol &scope,
                          const codesh::ast::block::conditioned_scope_container &conditioned_scope)
{
    bool all_succeed = true;
    all_succeed &= resolve_value(context, *conditioned_scope.condition, containing_method, scope);
    all_succeed &= resolve_scope(context, containing_method, conditioned_scope.scope);
    all_succeed &= is_condition_boolean(*conditioned_scope.condition);
    return all_succeed;
}

static bool is_primitive_type(const codesh::ast::var_reference::value_ast_node &val_node,
                              const codesh::definition::primitive_type type,
                              const std::string &blasphemy_details)
{
    if (const auto prim_type = dynamic_cast<codesh::ast::type::primitive_type_ast_node *>(val_node.get_type()))
    {
        if (prim_type->get_type() == type)
            return true;
    }

    codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
        blasphemy_details,
        codesh::blasphemy::blasphemy_type::SEMANTIC
    );
    return false;
}

static bool is_condition_boolean(const codesh::ast::var_reference::value_ast_node &val_node)
{
    return is_primitive_type(
        val_node,
        codesh::definition::primitive_type::BOOLEAN,
        codesh::blasphemy::details::CONDITION_NOT_BOOLEAN
    );
}
