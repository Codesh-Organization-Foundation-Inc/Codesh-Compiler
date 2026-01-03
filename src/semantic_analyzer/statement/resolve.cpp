#include "resolve.h"
#include "method_call/resolve.h"
#include "variable_reference/resolve.h"

#include "../../parser/ast/var_reference/variable_reference_ast_node.h"
#include "../../semantic_analyzer/symbol_table/symbol.h"

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

    throw std::runtime_error("Unsupported statement");
}
