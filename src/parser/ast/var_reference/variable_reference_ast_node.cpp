#include "variable_reference_ast_node.h"

#include "output/jvm_target/constant_pool.h"
#include "output/ir/code_block.h"
#include "semantic_analyzer/symbol_table/symbol.h"

variable_reference_ast_node::variable_reference_ast_node(codesh::definition::fully_qualified_name name) :
    name(std::move(name))
{
}

variable_reference_ast_node::variable_reference_ast_node(
        const codesh::ast::local_variable_declaration_ast_node &producing_declaration) :
    name(codesh::definition::fully_qualified_name(producing_declaration.get_name())),
    producing_declaration(producing_declaration)
{
}

const std::optional<std::reference_wrapper<codesh::semantic_analyzer::variable_symbol>> &variable_reference_ast_node::
    _get_resolved() const
{
    return resolved_symbol;
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

const codesh::definition::fully_qualified_name &variable_reference_ast_node::get_unresolved_name() const
{
    return name;
}

std::optional<std::reference_wrapper<const codesh::ast::local_variable_declaration_ast_node>>
    variable_reference_ast_node::get_producing_declaration() const
{
    return producing_declaration;
}

void variable_reference_ast_node::emit_constants(const codesh::ast::compilation_unit_ast_node &root_node,
                                                 codesh::output::jvm_target::constant_pool &constant_pool)
{
    if (const auto field = dynamic_cast<const codesh::semantic_analyzer::field_symbol *>(&get_resolved()))
    {
        field_cpi = constant_pool.goc_fieldref_info(
            constant_pool.goc_class_info(
                constant_pool.goc_utf8_info(field->get_full_name().omit_last().join())
            ),

            constant_pool.goc_name_and_type_info(
                constant_pool.goc_utf8_info(field->get_full_name().get_last_part()),
                constant_pool.goc_utf8_info(get_resolved().get_type()->generate_descriptor())
            )
        );
    }
}

void variable_reference_ast_node::emit_ir(
    codesh::output::ir::code_block &containing_block, const codesh::semantic_analyzer::symbol_table &symbol_table,
    const codesh::ast::type_decl::type_declaration_ast_node &containing_type_decl) const
{
    if (field_cpi.has_value())
    {
        //TODO: Expand beyond static
        containing_block.add_instruction(std::make_unique<codesh::output::ir::get_static_instruction>(*field_cpi));
    }
    else if (const auto local_var = dynamic_cast<const codesh::semantic_analyzer::local_variable_symbol *>(&get_resolved()))
    {
        containing_block.add_instruction(std::make_unique<codesh::output::ir::load_instruction>(
            local_var->get_type()->to_instruction_type(),
            local_var->get_jvm_index()
        ));
    }
    else
    {
        throw std::runtime_error("Unsupported variable reference type");
    }
}
