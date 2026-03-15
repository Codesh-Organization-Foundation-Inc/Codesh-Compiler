#include "local_variable_declaration_ast_node.h"

#include "output/jvm_target/constant_pool.h"
#include "semantic_analyzer/symbol_table/symbol.h"

const std::optional<std::reference_wrapper<codesh::semantic_analyzer::local_variable_symbol>> &codesh::ast::
    local_variable_declaration_ast_node::_get_resolved() const
{
    return resolved_variable;
}

codesh::ast::local_variable_declaration_ast_node::local_variable_declaration_ast_node(
        const blasphemy::code_position code_position) :
    variable_declaration_ast_node(code_position),
    accessible_from(-1),
    accessible_to(-1)
{
}

void codesh::ast::local_variable_declaration_ast_node::set_resolved(semantic_analyzer::local_variable_symbol &symbol)
{
    resolved_variable.emplace(symbol);
}

size_t codesh::ast::local_variable_declaration_ast_node::get_accessible_to() const
{
    return accessible_to;
}

void codesh::ast::local_variable_declaration_ast_node::set_accessible_to(const size_t accessible_to)
{
    this->accessible_to = accessible_to;
}

size_t codesh::ast::local_variable_declaration_ast_node::get_accessible_from() const
{
    return accessible_from;
}

void codesh::ast::local_variable_declaration_ast_node::set_accessible_from(const size_t accessible_from)
{
    this->accessible_from = accessible_from;
}

size_t codesh::ast::local_variable_declaration_ast_node::get_bytecode_start_pc() const
{
    return bytecode_start_pc;
}

void codesh::ast::local_variable_declaration_ast_node::set_bytecode_start_pc(const size_t start_pc)
{
    this->bytecode_start_pc = start_pc;
}

size_t codesh::ast::local_variable_declaration_ast_node::get_bytecode_length() const
{
    return bytecode_length;
}

void codesh::ast::local_variable_declaration_ast_node::set_bytecode_length(const size_t length)
{
    this->bytecode_length = length;
}
