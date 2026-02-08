#pragma once

#include "marker_instruction.h"

namespace codesh::ast::method
{
class method_scope_ast_node;
}

namespace codesh::output::ir
{

/**
 * Marker instruction for tracking scope boundaries.
 * Does not emit any bytecode, but helps track bytecode positions.
 *
 * @note Used for LocalVariableTable and StackMapTable generation
 */
class scope_marker : public marker_instruction
{
    const ast::method::method_scope_ast_node &scope;
    size_t bytecode_position = 0;

public:
    explicit scope_marker(const ast::method::method_scope_ast_node &scope);

    [[nodiscard]] const ast::method::method_scope_ast_node &get_scope() const;
    [[nodiscard]] size_t get_bytecode_position() const;
    void set_bytecode_position(size_t pos);
};

/**
 * Marker instruction for tracking scope boundaries.
 * Does not emit any bytecode, but helps track bytecode positions.
 */
class scope_begin_marker final : public scope_marker
{
public:
    explicit scope_begin_marker(const ast::method::method_scope_ast_node &scope);
};

/**
 * Marker instruction for tracking scope boundaries.
 * Does not emit any bytecode, but helps track bytecode positions.
 */
class scope_end_marker final : public scope_marker
{
public:
    explicit scope_end_marker(const ast::method::method_scope_ast_node &scope);
};

}
