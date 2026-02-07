#include "for_ast_node.h"

#include "output/ir/instruction/assignment_from_code_block_instruction.h"
#include "output/ir/instruction/if_instruction.h"
#include "output/ir/instruction/increment_by_constant_instruction.h"
#include "output/ir/instruction/load_instruction.h"
#include "output/ir/instruction/store_in_local_var_instruction.h"
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


    if (const auto range = dynamic_cast<const collection::range_ast_node *>(collection.get()))
    {
        if (const auto evaluable = dynamic_cast<const var_reference::evaluable_ast_node<int> *>(&range->get_skip()))
        {
            const auto skip_constant = evaluable->get_value();

            if (skip_constant < std::numeric_limits<int16_t>::min()
                || skip_constant > std::numeric_limits<int16_t>::max())
            {
                skip_constant_cpi = constant_pool.goc_integer_info(skip_constant);
            }
        }
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

    // Initialize iterator to range start
    range->get_from().emit_ir(containing_block, symbol_table, containing_type_decl);

    containing_block.add_instruction(std::make_unique<output::ir::store_in_local_var_instruction>(
        iterator.get_type()->to_instruction_type(),
        iterator.get_resolved().get_jvm_index()
    ));


    // Emit body scope
    output::ir::code_block body_block;
    body_scope.emit_ir(body_block, symbol_table, containing_type_decl);


    // Handle skip
    const auto &skip = range->get_skip();
    const auto it_lvt_index = iterator.get_resolved().get_jvm_index();

    //TODO: Handle non-int
    if (const auto evaluable = dynamic_cast<const var_reference::evaluable_ast_node<int> *>(&skip))
    {
        const auto value = evaluable->get_value();

        body_block.add_instruction(std::make_unique<output::ir::increment_by_constant_instruction>(
            iterator.get_type()->to_instruction_type(), // Always int rn
            it_lvt_index,
            value,
            skip_constant_cpi
        ));
    }
    else
    {
        output::ir::code_block skip_block;
        skip.emit_ir(skip_block, symbol_table, containing_type_decl);

        body_block.add_instruction(std::make_unique<output::ir::assignment_from_code_block_instruction>(
            iterator.get_type()->to_instruction_type(),
            output::ir::operator_type::ADD,
            it_lvt_index,
            std::move(skip_block)
        ));
    }


    const size_t body_jump_size = body_block.size() + 3; // body + goto

    output::ir::code_block condition_block;

    // load iterator
    condition_block.add_instruction(std::make_unique<output::ir::load_instruction>(
        iterator.get_type()->to_instruction_type(), iterator.get_resolved().get_jvm_index()
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
}
