#pragma once

#include "parser/ast/var_reference/value_ast_node.h"

namespace codesh::ast::impl
{

template <int operands>
class operator_ast_node : public var_reference::value_ast_node, i_constant_pool_emitter
{
protected:
    std::unique_ptr<value_ast_node> children[operands]{};

public:
    using value_ast_node::value_ast_node;

    [[nodiscard]] virtual bool is_value_valid() const = 0;
    [[nodiscard]] virtual std::string to_pretty_string() const = 0;

    void set_statement_index(size_t statement_index) override;

    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
             const type_decl::type_declaration_ast_node &containing_type_decl) const override;

    void emit_constants(const compilation_unit_ast_node &root_node,
            output::jvm_target::constant_pool &constant_pool) override;
};

}

#include "operator_ast_node.tpp"
