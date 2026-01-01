#include "resolve.h"
#include "local_var_decl/resolve.h"
#include "method_call/resolve.h"

#include "../../semantic_analyzer/symbol_table/symbol.h"

void codesh::semantic_analyzer::statement::resolve(const semantic_context &context,
                                                   ast::method::operation::method_operation_ast_node &stmnt,
                                                   const method_symbol &containing_method)
{
    if (const auto var_decl = dynamic_cast<ast::local_variable_declaration_ast_node *>(&stmnt))
    {
        local_var_decl::resolve(context, *var_decl, containing_method);
    }
    else if (const auto method_call = dynamic_cast<ast::method::operation::method_call_ast_node *>(&stmnt))
    {
        method_call::resolve(context, *method_call, containing_method);
    }
}
