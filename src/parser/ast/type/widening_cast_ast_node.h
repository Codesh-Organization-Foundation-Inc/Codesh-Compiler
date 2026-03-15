#pragma once

#include "parser/ast/impl/i_constant_pool_emitter.h"
#include "parser/ast/type/primitive_type_ast_node.h"
#include "parser/ast/var_reference/value_ast_node.h"

#include <memory>

namespace codesh::ast::type_decl
{
class type_declaration_ast_node;
}

namespace codesh::ast::type
{

/**
 * An AST wrapper that widens an inner primitive value to a wider target primitive type.
 * Emits the inner IR first, then (if necessary) a widening conversion opcode.
 */
class widening_cast_ast_node final : public var_reference::value_ast_node, public impl::i_constant_pool_emitter
{
    std::unique_ptr<value_ast_node> child;
    std::unique_ptr<primitive_type_ast_node> target_type;

public:
    widening_cast_ast_node(blasphemy::code_position code_position, std::unique_ptr<value_ast_node> inner,
        std::unique_ptr<primitive_type_ast_node> target_type);

    [[nodiscard]] type_ast_node *get_type() const override;

    void set_statement_index(size_t statement_index) override;

    void emit_constants(const compilation_unit_ast_node &root_node,
                        output::jvm_target::constant_pool &constant_pool) override;

    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
        const ast::type_decl::type_declaration_ast_node &containing_type_decl) const override;
};

}
