#pragma once

#include "defenition/i_pretty_string_convertable.h"
#include "i_constant_pool_emitter.h"
#include "parser/ast/var_reference/value_ast_node.h"

namespace codesh::semantic_analyzer
{
struct semantic_context;
}
namespace codesh::ast::impl
{

template <int operands>
class operator_ast_node : public var_reference::value_ast_node, public i_constant_pool_emitter,
    public definition::i_pretty_string_convertable
{
protected:
    std::unique_ptr<value_ast_node> children[operands]{};

public:
    using value_ast_node::value_ast_node;

    [[nodiscard]] virtual bool is_value_valid(const semantic_analyzer::semantic_context &context) const = 0;

    void set_statement_index(size_t statement_index) override;

    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
             const type_decl::type_declaration_ast_node &containing_type_decl) const override;

    void emit_constants(const compilation_unit_ast_node &root_node,
            output::jvm_target::constant_pool &constant_pool) override;
};

}

#include "operator_ast_node.tpp"
