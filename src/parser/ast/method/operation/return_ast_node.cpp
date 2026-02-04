#include "return_ast_node.h"

#include "output/ir/code_block.h"

using namespace codesh::ast::method::operation;

return_ast_node::return_ast_node(
    blasphemy::code_position code_position,
    std::unique_ptr<var_reference::value_ast_node> return_value
)
    : method_operation_ast_node(code_position),
      return_value(std::move(return_value))
{
}

codesh::ast::var_reference::value_ast_node *return_ast_node::get_return_value() const
{
    return return_value.get();
}

void return_ast_node::set_statement_index(size_t statement_index)
{
    method_operation_ast_node::set_statement_index(statement_index);

    if (return_value)
    {
        return_value->set_statement_index(statement_index);
    }
}

void codesh::ast::method::operation::return_ast_node::emit_ir(
    output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
    const type_decl::type_declaration_ast_node &containing_type_decl) const
{
    containing_block.add_instruction(std::make_unique<output::ir::return_instruction>());
}
