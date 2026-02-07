#pragma once

#include "method_operation_ast_node.h"
#include "parser/ast/impl/i_constant_pool_emitter.h"
#include "parser/ast/impl/i_descriptor_emitter.h"
#include "parser/ast/impl/i_resolvable.h"
#include "parser/ast/var_reference/value_ast_node.h"

#include <deque>
#include <memory>

namespace codesh::ast
{
class compilation_unit_ast_node;
}
namespace codesh::semantic_analyzer
{
class method_symbol;
}

namespace codesh::ast::method::operation
{

class method_call_ast_node : public var_reference::value_ast_node,
    public impl::i_constant_pool_emitter,
    public impl::i_resolvable<semantic_analyzer::method_symbol>,
    public impl::i_descriptor_emitter,
    public definition::i_pretty_string_convertable
{
    definition::fully_qualified_name name;
    std::optional<std::reference_wrapper<semantic_analyzer::method_symbol>> resolved_symbol;

    std::deque<std::unique_ptr<value_ast_node>> arguments;

    static size_t determine_stack_delta(const type::type_ast_node &type);

protected:
    [[nodiscard]] const std::optional<std::reference_wrapper<semantic_analyzer::method_symbol>> &_get_resolved() const
        override;

public:
    explicit method_call_ast_node(blasphemy::code_position code_position);

    void set_resolved(semantic_analyzer::method_symbol &symbol) override;

    [[nodiscard]] const definition::fully_qualified_name &get_unresolved_name() const override;

    [[nodiscard]] definition::fully_qualified_name &get_fqn();
    [[nodiscard]] const definition::fully_qualified_name &get_fqn() const;


    [[nodiscard]] type::type_ast_node *get_type() const override;

    using i_descriptor_emitter::generate_descriptor;
    [[nodiscard]] std::string generate_descriptor(bool resolved) const override;


    [[nodiscard]] const std::deque<std::unique_ptr<value_ast_node>> &get_arguments() const;
    [[nodiscard]] std::deque<std::unique_ptr<value_ast_node>> &get_arguments();

    void set_statement_index(size_t statement_index) override;


    [[nodiscard]] std::string to_pretty_string() const override;


    void emit_constants(const compilation_unit_ast_node &root_node,
                        output::jvm_target::constant_pool &constant_pool) override;

    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
                 const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};

}
