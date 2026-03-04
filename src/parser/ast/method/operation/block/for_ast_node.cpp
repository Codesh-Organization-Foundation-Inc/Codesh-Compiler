#include "for_ast_node.h"

#include "output/ir/instruction/assignment_from_code_block_instruction.h"
#include "output/ir/instruction/if_instruction.h"
#include "output/ir/instruction/increment_int_by_constant_instruction.h"
#include "output/ir/instruction/load_instruction.h"
#include "output/ir/instruction/scope_marker.h"
#include "output/ir/instruction/store_in_local_var_instruction.h"
#include "output/ir/util.h"
#include "parser/ast/collection/range_ast_node.h"
#include "parser/ast/method/method_scope_ast_node.h"
#include "parser/ast/type_declaration/type_declaration_ast_node.h"
#include "parser/ast/var_reference/evaluable_ast_node.h"

codesh::ast::block::for_ast_node::for_ast_node(const blasphemy::code_position code_position,
                           std::unique_ptr<var_reference::value_ast_node> collection,
                           method::method_scope_ast_node &body_scope) :
    method_operation_ast_node(code_position),
    body_scope(body_scope),
    iterator(*body_scope.get_local_variables().at(0)),
    collection(std::move(collection))
{
}

codesh::ast::local_variable_declaration_ast_node &codesh::ast::block::for_ast_node::get_iterator() const
{
    return iterator;
}

codesh::ast::var_reference::value_ast_node &codesh::ast::block::for_ast_node::get_collection() const
{
    return *collection;
}

codesh::ast::method::method_scope_ast_node &codesh::ast::block::for_ast_node::get_body_scope() const
{
    return body_scope;
}

codesh::ast::method::method_scope_ast_node &codesh::ast::block::for_ast_node::get_iterator_declaration_scope() const
{
    return *body_scope.get_method_scopes().front();
}

void codesh::ast::block::for_ast_node::set_statement_index(const size_t statement_index)
{
    method_operation_ast_node::set_statement_index(statement_index);
    collection->set_statement_index(statement_index);
}

void codesh::ast::block::for_ast_node::emit_constants(const compilation_unit_ast_node &root_node,
        output::jvm_target::constant_pool &constant_pool)
{
    iterator.emit_constants(root_node, constant_pool);
    if (const auto cp_emitter = dynamic_cast<i_constant_pool_emitter *>(collection.get()))
    {
        cp_emitter->emit_constants(root_node, constant_pool);
    }
    body_scope.emit_constants(root_node, constant_pool);

    get_iterator_declaration_scope().emit_constants(root_node, constant_pool);


    if (const auto range = dynamic_cast<const collection::range_ast_node *>(collection.get()))
    {
        skip_constant_cpi = output::ir::util::goc_big_int_value(*range, constant_pool, output::ir::operator_type::ADD);
    }
}

void codesh::ast::block::for_ast_node::emit_ir(
        output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
        const type_decl::type_declaration_ast_node &containing_type_decl) const
{
    const auto range = dynamic_cast<collection::range_ast_node *>(collection.get());
    if (!range)
    {
        throw std::runtime_error("Non-range collections not currently supported");
    }

    const auto it_type = iterator.get_type()->to_instruction_type();
    const auto it_lvt = iterator.get_resolved().get_jvm_index();

    // Begin the iterator's scope BEFORE init so it's visible in the condition and body.
    // Thus, we emit markers manually INSTEAD of using body_scope.emit_ir() which would only
    // wrap the body statements.
    containing_block.add_instruction(std::make_unique<output::ir::scope_begin_marker>(body_scope));

    // Initialize the iterator to the range's start
    range->get_from().emit_ir(containing_block, symbol_table, containing_type_decl);

    containing_block.add_instruction(std::make_unique<output::ir::store_in_local_var_instruction>(
        it_type,
        it_lvt
    ));

    // Emit body variables scope: its scope_begin_marker lands inside the loop body
    // (after the condition check), so body-declared variables get a bytecode_start_pc
    // that correctly excludes the loop back-edge target.
    output::ir::code_block body_block;
    get_iterator_declaration_scope().emit_ir(body_block, symbol_table, containing_type_decl);

    // Emit the skip
    output::ir::util::emit_increment_by_value_optimized(
        body_block, symbol_table, containing_type_decl,
        range->get_skip(),
        {it_type, output::ir::operator_type::ADD, it_lvt},
        skip_constant_cpi
    );

    const size_t body_jump_size = body_block.size() + 3; // body + advance + goto

    output::ir::code_block condition_block;

    // load iterator
    condition_block.add_instruction(std::make_unique<output::ir::load_instruction>(
        it_type,
        it_lvt
    ));

    // load to
    range->get_to().emit_ir(condition_block, symbol_table, containing_type_decl);

    // compare (iterator >= to)
    condition_block.add_instruction(std::make_unique<output::ir::if_instruction>(
        output::ir::if_type::IS_INT_GREATER_OR_EQUAL,
        static_cast<int>(body_jump_size))
    );

    const size_t condition_size = condition_block.size();

    containing_block.consume_code_block(std::move(condition_block));
    containing_block.consume_code_block(std::move(body_block));

    // Jump back to condition
    containing_block.add_instruction(std::make_unique<output::ir::goto_instruction>(
        -static_cast<int>(condition_size + body_jump_size)
    ));

    // End the loop scope
    containing_block.add_instruction(std::make_unique<output::ir::scope_end_marker>(body_scope));
}
