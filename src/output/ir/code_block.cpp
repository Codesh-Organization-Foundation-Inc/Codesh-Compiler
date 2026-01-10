#include "code_block.h"

#include "parser/ast/method/method_declaration_ast_node.h"

codesh::output::ir::code_block codesh::output::ir::code_block::build_from_method(
    const ast::method::method_declaration_ast_node &method, const semantic_analyzer::symbol_table &symbol_table,
    const ast::type_decl::type_declaration_ast_node &containing_type)
{
    code_block result;

    for (const auto &method_op : method.get_method_scope().get_body())
    {
        const auto ir_emitter = dynamic_cast<ast::impl::i_ir_emitter *>(method_op.get());
        if (!ir_emitter)
            continue;

        ir_emitter->emit_ir(result, symbol_table, containing_type);
    }

    return result;
}

const std::list<std::unique_ptr<codesh::output::ir::instruction>> &codesh::output::ir::code_block::get_instructions()
    const
{
    return instructions;
}

void codesh::output::ir::code_block::add_instruction(std::unique_ptr<instruction> instruction)
{
    instructions.emplace_back(std::move(instruction));
}
