#pragma once

#include "parser/ast/var_reference/value_ast_node.h"
#include "i_constant_pool_emitter.h"
#include "operator_ast_node.h"

#include <memory>

namespace codesh::ast::impl
{

class unary_ast_node : public operator_ast_node, i_constant_pool_emitter
{
    std::unique_ptr<type::type_ast_node> type;
    std::unique_ptr<value_ast_node> child;

protected:
    unary_ast_node(blasphemy::code_position code_position, std::unique_ptr<value_ast_node> child);

public:
    [[nodiscard]] value_ast_node &get_child() const;
    [[nodiscard]] type::type_ast_node *get_type() const override;

    void set_statement_index(size_t statement_index) override;

    //TODO: Each node should specify this for THEMSELVES!!! This default should NOT exist
    [[nodiscard]] bool is_value_valid() const override;


    void emit_constants(const compilation_unit_ast_node &root_node,
            output::jvm_target::constant_pool &constant_pool) override;
};

}
