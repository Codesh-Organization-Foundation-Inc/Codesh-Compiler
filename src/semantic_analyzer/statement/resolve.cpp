#include "resolve.h"
#include "method_call/resolve.h"
#include "variable_reference/resolve.h"

#include "../../parser/ast/impl/binary_ast_node.h"
#include "../../parser/ast/impl/unary_ast_node.h"
#include "../../parser/ast/var_reference/variable_reference_ast_node.h"
#include "../../semantic_analyzer/symbol_table/symbol.h"

static bool resolve_if_var_ref(const codesh::semantic_analyzer::semantic_context &context,
        codesh::ast::var_reference::value_ast_node &val_node,
        const codesh::semantic_analyzer::method_scope_symbol &scope);


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


    if (const auto unary_op = dynamic_cast<ast::impl::unary_ast_node *>(&stmnt))
    {
        return resolve_if_var_ref(context, unary_op->get_child(), scope);
    }

    if (const auto binary_op = dynamic_cast<ast::impl::binary_ast_node *>(&stmnt))
    {
        bool all_succeed = true;
        all_succeed &= resolve_if_var_ref(context, binary_op->get_left(), scope);
        all_succeed &= resolve_if_var_ref(context, binary_op->get_right(), scope);
        return all_succeed;
    }

    // Probably doesn't need to be resolved.
    return true;
}


static bool resolve_if_var_ref(const codesh::semantic_analyzer::semantic_context &context,
        codesh::ast::var_reference::value_ast_node &val_node,
        const codesh::semantic_analyzer::method_scope_symbol &scope)
{
    const auto var_ref = dynamic_cast<variable_reference_ast_node *>(&val_node);
    if (var_ref == nullptr)
    {
        // "Already resolved"
        // (Only variable references need to be resolved)
        return true;
    }

    return codesh::semantic_analyzer::statement::variable_reference::resolve(context, *var_ref, scope);
}
