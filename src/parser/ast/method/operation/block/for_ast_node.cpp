#include "for_ast_node.h"

#include "parser/ast/method/method_scope_ast_node.h"

codesh::ast::block::for_ast_node::for_ast_node(const blasphemy::code_position code_position,
                           std::unique_ptr<local_variable_declaration_ast_node> iterator,
                           std::unique_ptr<var_reference::value_ast_node> collection,
                           method::method_scope_ast_node &body_scope) :
    method_operation_ast_node(code_position),
    iterator(std::move(iterator)),
    collection(std::move(collection)),
    body_scope(body_scope)
{
}

codesh::ast::local_variable_declaration_ast_node &codesh::ast::block::for_ast_node::get_iterator() const
{
    return *iterator;
}

codesh::ast::var_reference::value_ast_node &codesh::ast::block::for_ast_node::get_collection() const
{
    return *collection;
}

codesh::ast::method::method_scope_ast_node &codesh::ast::block::for_ast_node::get_body_scope() const
{
    return body_scope;
}

void codesh::ast::block::for_ast_node::set_statement_index(size_t statement_index)
{
    method_operation_ast_node::set_statement_index(statement_index);
    collection->set_statement_index(statement_index);
}

void codesh::ast::block::for_ast_node::emit_constants(const compilation_unit_ast_node &root_node,
        output::jvm_target::constant_pool &constant_pool)
{

    iterator->emit_constants(root_node, constant_pool);
    if (const auto cp_emitter = dynamic_cast<i_constant_pool_emitter *>(collection.get()))
    {
        cp_emitter->emit_constants(root_node, constant_pool);
    }
    body_scope.emit_constants(root_node, constant_pool);
}

void codesh::ast::block::for_ast_node::emit_ir(
    output::ir::code_block &containing_block,
    const semantic_analyzer::symbol_table &symbol_table,
    const type_decl::type_declaration_ast_node &containing_type_decl
) const
{
//     // 1. Initialize iterator: iterator = range.from
//     iterator->emit_ir(containing_block, symbol_table, containing_type_decl);
//
//     // 2. Prepare loop body + increment in a temp block
//     output::ir::code_block body_block;
//
//     // Emit loop body
//     body_scope.emit_ir(body_block, symbol_table, containing_type_decl);
//
//     // iterator = iterator + range.skip
//     {
//         auto increment = std::make_unique<ast::op::addition_operator_ast_node>(
//             iterator->get_code_position(),
//             std::make_unique<ast::var_reference::variable_reference_ast_node>(
//                 iterator->get_code_position(),
//                 iterator->get_name()
//             ),
//             static_cast<const ast::collection::range_ast_node &>(*collection).get_skip().clone()
//         );
//
//         increment->emit_ir(body_block, symbol_table, containing_type_decl);
//     }
//
//     const size_t body_size = body_block.size();
//     const size_t body_jump_size = body_size + 3; // body + goto
//
//     // 3. Build condition: iterator < range.to
//     const auto &range = static_cast<const ast::collection::range_ast_node &>(*collection);
//
//     auto condition = std::make_unique<ast::op::less_operator_ast_node>(
//         iterator->get_code_position(),
//         std::make_unique<ast::var_reference::variable_reference_ast_node>(
//             iterator->get_code_position(),
//             iterator->get_name()
//         ),
//         range.get_to().clone()
//     );
//
//     auto condition_block = output::ir::build_condition_block(
//         *condition,
//         body_jump_size,
//         symbol_table,
//         containing_type_decl,
//         output::ir::if_type::IS_ZERO
//     );
//
//     const size_t condition_size = condition_block.size();
//
//     // 4. Stitch blocks together
//     containing_block.consume_code_block(std::move(condition_block));
//     containing_block.consume_code_block(std::move(body_block));
//
//     // 5. Jump back to condition
//     containing_block.add_instruction(
//         std::make_unique<output::ir::goto_instruction>(
//             -static_cast<int>(condition_size + body_size + 3)
//         )
//     );
}
