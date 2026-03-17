#include "return_ast_node.h"

#include "output/ir/code_block.h"
#include "output/ir/instruction/return_instruction.h"
#include "parser/ast/type/custom_type_ast_node.h"
#include "parser/ast/type/primitive_type_ast_node.h"

codesh::ast::method::operation::return_ast_node::return_ast_node(
        const lexer::code_position code_position,
        std::unique_ptr<var_reference::value_ast_node> return_value) :
    method_operation_ast_node(code_position),
    return_value(std::move(return_value))
{
}

codesh::ast::var_reference::value_ast_node *codesh::ast::method::operation::return_ast_node::get_return_value() const
{
    return return_value.get();
}

std::unique_ptr<codesh::ast::var_reference::value_ast_node> codesh::ast::method::operation::return_ast_node::
    take_return_value()
{
    return std::move(return_value);
}

void codesh::ast::method::operation::return_ast_node::set_return_value(
    std::unique_ptr<var_reference::value_ast_node> new_value)
{
    return_value = std::move(new_value);
}

void codesh::ast::method::operation::return_ast_node::set_statement_index(const size_t statement_index)
{
    method_operation_ast_node::set_statement_index(statement_index);

    if (return_value)
    {
        return_value->set_statement_index(statement_index);
    }
}

void codesh::ast::method::operation::return_ast_node::emit_constants(const compilation_unit_ast_node &root_node,
                                                                     output::jvm_target::constant_pool &constant_pool)
{
    if (const auto cp_emitter = dynamic_cast<i_constant_pool_emitter *>(return_value.get()))
    {
        cp_emitter->emit_constants(root_node, constant_pool);
    }
}

void codesh::ast::method::operation::return_ast_node::emit_ir(
    output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
    const type_decl::type_declaration_ast_node &containing_type_decl) const
{
    if (!return_value)
    {
        containing_block.add_instruction(std::make_unique<output::ir::return_instruction>());
    }
    else
    {
        return_value->emit_ir(containing_block, symbol_table, containing_type_decl);

        containing_block.add_instruction(std::make_unique<output::ir::return_instruction>(
            get_opcode_return_type(*return_value->get_type())
        ));
    }
}

codesh::output::ir::instruction_type codesh::ast::method::operation::return_ast_node::get_opcode_return_type(
    const type::type_ast_node &return_type)
{
    if (dynamic_cast<const type::custom_type_ast_node *>(&return_type))
    {
        return output::ir::instruction_type::REFERENCE;
    }
    if (const auto primitive_type = dynamic_cast<const type::primitive_type_ast_node *>(&return_type))
    {
        return primitive_type->to_instruction_type();
    }

    throw std::invalid_argument("Unsupported type");
}