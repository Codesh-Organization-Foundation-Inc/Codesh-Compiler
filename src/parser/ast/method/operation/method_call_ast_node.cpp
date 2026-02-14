#include "method_call_ast_node.h"

#include "defenition/definitions.h"
#include "fmt/xchar.h"
#include "output/ir/code_block.h"
#include "output/ir/instruction/invoke_instruction.h"
#include "output/ir/instruction/marker_instruction.h"
#include "parser/ast/method/util.h"
#include "parser/ast/type/custom_type_ast_node.h"
#include "parser/ast/type/primitive_type_ast_node.h"
#include "parser/ast/var_reference/evaluable_ast_node.h"
#include "semantic_analyzer/symbol_table/symbol.h"

const std::optional<std::reference_wrapper<codesh::semantic_analyzer::method_symbol>> &codesh::ast::method::operation::
    method_call_ast_node::_get_resolved() const
{
    return resolved_symbol;
}

codesh::ast::method::operation::method_call_ast_node::method_call_ast_node(
        const blasphemy::code_position code_position) :
    value_ast_node(code_position)
{
}

void codesh::ast::method::operation::method_call_ast_node::set_resolved(semantic_analyzer::method_symbol &symbol)
{
    resolved_symbol.emplace(symbol);
}

const codesh::definition::fully_qualified_name &codesh::ast::method::operation::method_call_ast_node::get_unresolved_name()
    const
{
    return name;
}

codesh::definition::fully_qualified_name &codesh::ast::method::operation::method_call_ast_node::get_fqn()
{
    return name;
}

const codesh::definition::fully_qualified_name &codesh::ast::method::operation::method_call_ast_node::get_fqn()
    const
{
    return name;
}

codesh::ast::type::type_ast_node *codesh::ast::method::operation::method_call_ast_node::get_type() const
{
    return &get_resolved().get_return_type();
}

std::string codesh::ast::method::operation::method_call_ast_node::generate_descriptor(const bool resolved) const
{
    if (!resolved)
    {
        throw std::runtime_error("Unable to generate unresolved descriptors for method calls");
    }


    const auto &method = get_resolved();

    std::deque<std::reference_wrapper<type::type_ast_node>> param_types;
    for (const auto &param : method.get_parameter_types())
    {
        param_types.emplace_back(*param);
    }

    return util::generate_method_descriptor(true, method.get_return_type(), param_types, method.get_attributes());
}

const std::deque<std::unique_ptr<codesh::ast::var_reference::value_ast_node>> &codesh::ast::method::operation::
    method_call_ast_node::get_arguments() const
{
    return arguments;
}

std::deque<std::unique_ptr<codesh::ast::var_reference::value_ast_node>> &codesh::ast::method::operation::
    method_call_ast_node::get_arguments()
{
    return arguments;
}

void codesh::ast::method::operation::method_call_ast_node::set_statement_index(const size_t statement_index)
{
    method_operation_ast_node::set_statement_index(statement_index);

    for (const auto &argument : get_arguments())
    {
        argument->set_statement_index(statement_index);
    }
}

std::string codesh::ast::method::operation::method_call_ast_node::to_pretty_string() const
{
    return fmt::format(
        definition::METHOD_PRETTY_STRING,
        get_last_name(false)
    );
}

void codesh::ast::method::operation::method_call_ast_node::emit_constants(
    const compilation_unit_ast_node &root_node, output::jvm_target::constant_pool &constant_pool)
{
    constant_pool.goc_methodref_info(
        constant_pool.goc_class_info(
            constant_pool.goc_utf8_info(get_resolved_name().omit_last().join())
        ),

        constant_pool.goc_name_and_type_info(
            constant_pool.goc_utf8_info(get_last_name(true)),
            constant_pool.goc_utf8_info(generate_descriptor())
        )
    );

    // Emit arguments
    for (const auto &argument : get_arguments())
    {
        if (const auto constant_emitter = dynamic_cast<i_constant_pool_emitter *>(argument.get()))
        {
            constant_emitter->emit_constants(root_node, constant_pool);
        }
    }
}

void codesh::ast::method::operation::method_call_ast_node::emit_ir(
    output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
    const type_decl::type_declaration_ast_node &containing_type_decl) const
{
    const auto &method = get_resolved();
    const auto &cp = containing_type_decl.get_constant_pool();

    if (!method.get_attributes().get_is_static())
    {
        //TODO: Handle passing 'this' instances
        throw std::runtime_error("Calling non-static methods not yet supported");
    }

    // Load arguments
    for (const auto &argument : arguments)
    {
        containing_block.set_is_consuming(true);
        argument->emit_ir(containing_block, symbol_table, containing_type_decl);
    }
    containing_block.set_is_consuming(false);

    // Call method
    const int method_cpi = cp.get_methodref_index(
        cp.get_class_index(
            cp.get_utf8_index(get_resolved_name().omit_last().join())
        ),

        cp.get_name_and_type_index(
            cp.get_utf8_index(get_last_name(true)),
            cp.get_utf8_index(generate_descriptor())
        )
    );


    //FIXME: Specifically make sout calls virtual because no non-static support yet exists
    const bool is_sout = get_unresolved_name().join() == "מסוף/אמר";

    containing_block.add_instruction(std::make_unique<output::ir::invoke_instruction>(
        is_sout ? output::ir::invokation_type::VIRTUAL : output::ir::invokation_type::STATIC,
        method_cpi,
        arguments.size()
    ));


    // If the method call has returned something, we should update the stack delta accordingly.
    const auto &return_type = method.get_return_type();
    const auto stack_delta = determine_stack_delta(return_type);

    if (stack_delta != 0)
    {
        containing_block.add_instruction(std::make_unique<output::ir::stack_size_delta_marker>(stack_delta));
    }
}

size_t codesh::ast::method::operation::method_call_ast_node::determine_stack_delta(const type::type_ast_node &type)
{
    if (const auto primitive_type = dynamic_cast<const type::primitive_type_ast_node *>(&type))
    {
        switch (primitive_type->get_type())
        {
        case definition::primitive_type::VOID:
            return 0;

        case definition::primitive_type::LONG:
        case definition::primitive_type::DOUBLE:
            return 2;

        default:
            return 1;
        }
    }

    if (dynamic_cast<const type::custom_type_ast_node *>(&type))
    {
        return 1;
    }

    throw std::invalid_argument("Unknown type");
}
