#include "array_access_ast_node.h"
#include "output/ir/code_block.h"
#include "output/ir/instruction/array_load_instruction.h"

codesh::ast::op::array_access_ast_node::array_access_ast_node(
        const lexer::code_position code_position,
        std::unique_ptr<value_ast_node> array,
        std::unique_ptr<value_ast_node> index) :
    value_ast_node(code_position),
    array(std::move(array)),
    index(std::move(index))
{
}

codesh::ast::var_reference::value_ast_node& codesh::ast::op::array_access_ast_node::get_array() const
{
    return *array;
}

codesh::ast::var_reference::value_ast_node& codesh::ast::op::array_access_ast_node::get_index() const
{
    return *index;
}

void codesh::ast::op::array_access_ast_node::set_element_type(std::unique_ptr<type::type_ast_node> type)
{
    element_type = std::move(type);
}

codesh::ast::type::type_ast_node* codesh::ast::op::array_access_ast_node::get_type() const
{
    return element_type.get();
}

void codesh::ast::op::array_access_ast_node::emit_ir(output::ir::code_block &containing_block,
        const semantic_analyzer::symbol_table &symbol_table,
        const type_decl::type_declaration_ast_node &containing_type_decl) const
{
    array->emit_ir(containing_block, symbol_table, containing_type_decl);
    index->emit_ir(containing_block, symbol_table, containing_type_decl);

    containing_block.add_instruction(
        std::make_unique<output::ir::array_load_instruction>(element_type->to_instruction_type())
    );
}
