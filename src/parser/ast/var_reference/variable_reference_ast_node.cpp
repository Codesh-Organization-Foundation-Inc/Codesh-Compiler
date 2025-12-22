#include "variable_reference_ast_node.h"

#include "../../../semantic_analyzer/symbol_table/symbol.h"

variable_reference_ast_node::variable_reference_ast_node(codesh::definition::fully_qualified_class_name name) :
    name(std::move(name))
{
}

const std::optional<std::reference_wrapper<codesh::semantic_analyzer::variable_symbol>> &variable_reference_ast_node::
    _get_resolved() const
{
    return resolved_symbol;
}

const codesh::definition::fully_qualified_class_name &variable_reference_ast_node::get_unresolved_name() const
{
    return name;
}

void variable_reference_ast_node::set_resolved(codesh::semantic_analyzer::variable_symbol &symbol)
{
    resolved_symbol.emplace(symbol);
}

codesh::ast::type::type_ast_node *variable_reference_ast_node::get_type() const
{
    if (!_get_resolved().has_value())
        throw std::runtime_error("Variable reference was not resolved, yet its type was attempted to be used");

    return get_resolved().get_type();
}
