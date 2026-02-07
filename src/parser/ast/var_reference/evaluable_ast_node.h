#pragma once

#include "parser/ast/impl/i_constant_pool_emitter.h"
#include "parser/ast/type/type_ast_node.h"
#include "value_ast_node.h"

namespace codesh::ast::var_reference
{

template <typename T>
class evaluable_ast_node : public value_ast_node, public impl::i_constant_pool_emitter
{
    const std::unique_ptr<type::type_ast_node> type;
    T value;

    std::optional<int> value_cpi;

public:
    evaluable_ast_node(blasphemy::code_position code_position, std::unique_ptr<type::type_ast_node> type, T value);

    [[nodiscard]] type::type_ast_node *get_type() const override;

    /**
     * @return The value.
     * @note If this is a reference, refer to the symbol table for more information on the passed type.
     */
    [[nodiscard]] T get_value() const;
    void set_value(T value);


    void emit_constants(const compilation_unit_ast_node &root_node,
                        output::jvm_target::constant_pool &constant_pool) override;

    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
                 const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};

}

#include "evaluable_ast_node.tpp"
