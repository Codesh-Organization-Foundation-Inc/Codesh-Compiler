#pragma once

#include "semantic_analyzer/symbol_table/symbol.h"

#include <optional>

namespace codesh::ast
{
class local_variable_declaration_ast_node;
}
namespace codesh::ast::method::operation
{
class method_operation_ast_node;
class method_call_ast_node;
}

namespace codesh::semantic_analyzer
{
struct semantic_context;
}

namespace codesh::semantic_analyzer
{
struct method_scope_info
{
    const method_symbol &method;
    const method_scope_symbol &scope;
};
}

namespace codesh::semantic_analyzer::statement
{
bool resolve(const semantic_context &context, ast::method::operation::method_operation_ast_node &stmnt,
        const std::optional<method_scope_info> &method_info);
}
