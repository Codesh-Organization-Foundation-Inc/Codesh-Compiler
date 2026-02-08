#include "scope_marker.h"

codesh::output::ir::scope_marker::scope_marker(const ast::method::method_scope_ast_node &scope) :
    scope(scope)
{
}

const codesh::ast::method::method_scope_ast_node &codesh::output::ir::scope_marker::get_scope() const
{
    return scope;
}

size_t codesh::output::ir::scope_marker::get_bytecode_position() const
{
    return bytecode_position;
}

void codesh::output::ir::scope_marker::set_bytecode_position(const size_t pos)
{
    bytecode_position = pos;
}

codesh::output::ir::scope_begin_marker::scope_begin_marker(const ast::method::method_scope_ast_node &scope) :
        scope_marker(scope)
{
}

codesh::output::ir::scope_end_marker::scope_end_marker(const ast::method::method_scope_ast_node &scope) :
        scope_marker(scope)
{
}
