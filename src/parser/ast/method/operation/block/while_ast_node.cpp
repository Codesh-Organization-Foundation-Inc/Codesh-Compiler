
#include "while_ast_node.h"

#include "output/ir/condition_block_builder.h"
#include "output/ir/instruction/goto_instruction.h"
#include "parser/ast/method/method_scope_ast_node.h"

codesh::ast::block::while_ast_node::while_ast_node(const blasphemy::code_position code_position,
        std::unique_ptr<var_reference::value_ast_node> condition,
        method::method_scope_ast_node &body_scope) :
    method_operation_ast_node(code_position),
    condition(std::move(condition)),
    body_scope(body_scope)
{
}

codesh::ast::var_reference::value_ast_node &codesh::ast::block::while_ast_node::get_condition() const
{
    return *condition;
}

codesh::ast::method::method_scope_ast_node &codesh::ast::block::while_ast_node::get_body_scope() const
{
    return body_scope;
}

void codesh::ast::block::while_ast_node::set_statement_index(const size_t statement_index)
{
    method_operation_ast_node::set_statement_index(statement_index);
    condition->set_statement_index(statement_index);
}

void codesh::ast::block::while_ast_node::emit_constants(const compilation_unit_ast_node &root_node,
                                    output::jvm_target::constant_pool &constant_pool)
{
    if (const auto cp_emitter = dynamic_cast<i_constant_pool_emitter *>(condition.get()))
    {
        cp_emitter->emit_constants(root_node, constant_pool);
    }
    body_scope.emit_constants(root_node, constant_pool);
}

void codesh::ast::block::while_ast_node::emit_ir(output::ir::code_block &containing_block,
        const semantic_analyzer::symbol_table &symbol_table,
        const type_decl::type_declaration_ast_node &containing_type_decl) const
{
    // Emit body into temp code_block to get its size before we emit it
    output::ir::code_block temp_block;
    body_scope.emit_ir(temp_block, symbol_table, containing_type_decl);

    const size_t body_size = temp_block.size();
    const size_t body_jump_size = body_size + 3; // (body + goto)

    auto condition_block = output::ir::build_condition_block(
        *condition,
        body_jump_size,
        symbol_table,
        containing_type_decl,
        output::ir::if_type::IS_ZERO
    );

    const size_t condition_size = condition_block.size();

    containing_block.consume_code_block(std::move(condition_block));
    containing_block.consume_code_block(std::move(temp_block));

    // Add goto back to loop start
    containing_block.add_instruction(
        std::make_unique<output::ir::goto_instruction>(
            // goto adds 3 opcodes, so -(condition_size + body_size + 3)
            -static_cast<int>(condition_size + body_size + 3)
        )
    );
}
