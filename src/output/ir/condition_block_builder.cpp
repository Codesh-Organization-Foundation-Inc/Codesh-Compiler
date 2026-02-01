#include "output/ir/condition_block_builder.h"

#include "parser/ast/operator/boolean/and_operator_ast_node.h"
#include "parser/ast/operator/boolean/or_operator_ast_node.h"

codesh::output::ir::code_block codesh::output::ir::build_condition_block(
        const ast::var_reference::value_ast_node &condition,
        const size_t if_block_size,
        const semantic_analyzer::symbol_table &symbol_table,
        const ast::type_decl::type_declaration_ast_node &containing_type_decl,
        const if_type if_type)
{
    code_block condition_block;

    // Handle AND
    if (const auto and_cond = dynamic_cast<const ast::op::and_operator_ast_node *>(&condition))
    {
        auto right_block = build_condition_block(
            and_cond->get_right(),
            if_block_size,
            symbol_table,
            containing_type_decl,
            if_type
        );

        size_t jump_size;
        if (if_type == if_type::IS_NONZERO)
        {
            // When jumping on true, short-circuit false by skipping the right block
            jump_size = right_block.size();
        }
        else
        {
            // When jumping on false, short-circuit false by skipping the right block and body
            jump_size = if_block_size + right_block.size();
        }

        auto left_block = build_condition_block(
            and_cond->get_left(),
            jump_size,
            symbol_table,
            containing_type_decl,
            if_type::IS_ZERO
        );

        condition_block.consume_code_block(std::move(left_block));
        condition_block.consume_code_block(std::move(right_block));
        return condition_block;
    }
    // Handle OR
    if (const auto or_cond = dynamic_cast<const ast::op::or_operator_ast_node *>(&condition))
    {
        auto right_block = build_condition_block(
            or_cond->get_right(),
            if_block_size,
            symbol_table,
            containing_type_decl,
            if_type
        );

        size_t jump_size;
        if (if_type == if_type::IS_ZERO)
        {
            // When jumping on false, short-circuit true by skipping over the right block
            jump_size = right_block.size();
        }
        else
        {
            // When jumping on true, short-circuit true by jumping immediately to the expression
            jump_size = if_block_size;
        }

        auto left_block = build_condition_block(
            or_cond->get_left(),
            jump_size,
            symbol_table,
            containing_type_decl,
            if_type::IS_NONZERO
        );

        condition_block.consume_code_block(std::move(left_block));
        condition_block.consume_code_block(std::move(right_block));
        return condition_block;
    }


    //TODO: This should only be emitted if no other if options exists (see next TODO)
    condition.emit_ir(condition_block, symbol_table, containing_type_decl);
    condition_block.add_instruction(std::make_unique<if_instruction>(
        if_type,
        static_cast<int>(if_block_size)
    ));

    //TODO: Add more if types (bytecode optimizations)
    // if (const auto &primitive_type = dynamic_cast<const type::primitive_type_ast_node *>(cond.get_type()))
    // {
    //     if (primitive_type->get_type() == definition::primitive_type::BOOLEAN)
    //     {
    //     }
    // }

    return condition_block;
}
