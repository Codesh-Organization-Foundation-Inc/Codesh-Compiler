#include "variable_reference_ast_node.h"

#include "output/ir/code_block.h"
#include "output/ir/instruction/get_field_instruction.h"
#include "output/ir/instruction/get_static_instruction.h"
#include "output/ir/instruction/load_instruction.h"
#include "output/jvm_target/constant_pool.h"
#include "semantic_analyzer/symbol_table/symbol.h"

codesh::ast::var_reference::variable_reference_ast_node::variable_reference_ast_node(
        const lexer::code_position code_position,
        definition::fully_qualified_name name) :
    value_ast_node(code_position),
    name(std::move(name)),
    association(reference_association::UNKNOWN)
{
}

codesh::ast::var_reference::variable_reference_ast_node::variable_reference_ast_node(
        const lexer::code_position code_position,
        const local_variable_declaration_ast_node &producing_declaration) :
    value_ast_node(code_position),
    name(definition::fully_qualified_name(code_position, producing_declaration.get_name())),
    association(reference_association::UNKNOWN),
    producing_declaration(producing_declaration)
{
}

const std::optional<std::reference_wrapper<codesh::semantic_analyzer::variable_symbol>> &codesh::ast::var_reference::
    variable_reference_ast_node::_get_resolved() const
{
    return resolved_symbol;
}

void codesh::ast::var_reference::variable_reference_ast_node::set_resolved(semantic_analyzer::variable_symbol &symbol)
{
    resolved_symbol.emplace(symbol);
}

codesh::ast::type::type_ast_node *codesh::ast::var_reference::variable_reference_ast_node::get_type() const
{
    if (!_get_resolved().has_value())
        throw std::runtime_error("Variable reference was not resolved, yet its type was attempted to be used");

    return get_resolved().get_type();
}

void codesh::ast::var_reference::variable_reference_ast_node::set_association(reference_association association)
{
    this->association = association;
}

codesh::ast::var_reference::reference_association codesh::ast::var_reference::variable_reference_ast_node::
    get_association() const
{
    return association;
}

const codesh::definition::fully_qualified_name &codesh::ast::var_reference::variable_reference_ast_node::get_unresolved_name() const
{
    return name;
}

std::optional<std::reference_wrapper<const codesh::ast::local_variable_declaration_ast_node>>
    codesh::ast::var_reference::variable_reference_ast_node::get_producing_declaration() const
{
    return producing_declaration;
}

std::optional<int> codesh::ast::var_reference::variable_reference_ast_node::get_field_cpi() const
{
    return field_cpi;
}

void codesh::ast::var_reference::variable_reference_ast_node::emit_constants(const compilation_unit_ast_node &root_node,
                                                 output::jvm_target::constant_pool &constant_pool)
{
    if (const auto field = dynamic_cast<const semantic_analyzer::field_symbol *>(&get_resolved()))
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

void codesh::ast::var_reference::variable_reference_ast_node::emit_ir(
    output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
    const type_decl::type_declaration_ast_node &containing_type_decl) const
{
    if (field_cpi.has_value())
    {
        const auto *field = dynamic_cast<const semantic_analyzer::field_symbol *>(&get_resolved());
        if (field->get_attributes().get_is_static())
        {
            containing_block.add_instruction(
                std::make_unique<output::ir::get_static_instruction>(*field_cpi));
        }
        else
        {
            // Push 'this' (local var slot 0), then read the instance field
            containing_block.add_instruction(std::make_unique<output::ir::load_instruction>(
                output::ir::instruction_type::REFERENCE, 0));
            containing_block.add_instruction(
                std::make_unique<output::ir::get_field_instruction>(*field_cpi));
        }
    }
    else if (const auto local_var = dynamic_cast<const semantic_analyzer::local_variable_symbol *>(&get_resolved()))
    {
        containing_block.add_instruction(std::make_unique<output::ir::load_instruction>(
            local_var->get_type()->to_instruction_type(),
            local_var->get_jvm_index()
        ));
    }
    else
    {
        throw std::runtime_error("Unsupported variable reference type");
    }
}
