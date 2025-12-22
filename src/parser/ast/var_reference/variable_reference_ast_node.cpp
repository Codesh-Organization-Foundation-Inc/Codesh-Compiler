#include "variable_reference_ast_node.h"

#include "../../../output/ir/code_block.h"
#include "../../../semantic_analyzer/symbol_table/symbol.h"

variable_reference_ast_node::variable_reference_ast_node(codesh::definition::fully_qualified_class_name name) :
    name(std::move(name))
{
}

const std::optional<std::reference_wrapper<codesh::semantic_analyzer::field_symbol>> &variable_reference_ast_node::
    _get_resolved() const
{
    return resolved_symbol;
}

const codesh::definition::fully_qualified_class_name &variable_reference_ast_node::get_unresolved_name() const
{
    return name;
}

void variable_reference_ast_node::set_resolved(codesh::semantic_analyzer::field_symbol &symbol)
{
    resolved_symbol.emplace(symbol);
}

codesh::ast::type::type_ast_node *variable_reference_ast_node::get_type() const
{
    if (!_get_resolved().has_value())
        throw std::runtime_error("Variable reference was not resolved, yet its type was attempted to be used");

    return get_resolved().get_type();
}

void variable_reference_ast_node::emit_ir(
    codesh::output::ir::code_block &containing_block, const codesh::semantic_analyzer::symbol_table &symbol_table,
    const codesh::ast::type_decl::type_declaration_ast_node &containing_type_decl) const
{
    const auto &cp = containing_type_decl.get_constant_pool();

    //TODO: Expand beyond static
    containing_block.add_instruction(std::make_unique<codesh::output::ir::get_static_instruction>(
        cp.get_fieldref_index(
            cp.get_class_index(
                cp.get_class_index(cp.get_utf8_index(get_resolved_name().omit_last().join()))
            ),

            cp.get_name_and_type_index(
                cp.get_utf8_index(get_last_name(true)),
                cp.get_utf8_index(get_resolved().get_type()->generate_descriptor())
            )
        )
    ));
}
