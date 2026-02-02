#pragma once

#include "parser/ast/var_reference/value_ast_node.h"
#include "i_constant_pool_emitter.h"
#include "operator_ast_node.h"

#include <memory>

namespace codesh::ast::impl
{

class binary_ast_node : public operator_ast_node, public i_constant_pool_emitter
{
    const std::unique_ptr<value_ast_node> left;
    const std::unique_ptr<value_ast_node> right;

    //FIXME: REMOVE THIS!!
    const std::unique_ptr<type::type_ast_node> type;

protected:
    binary_ast_node(blasphemy::code_position code_position, std::unique_ptr<value_ast_node> left,
            std::unique_ptr<value_ast_node> right);

public:
    [[nodiscard]] virtual value_ast_node &get_left() const;
    [[nodiscard]] value_ast_node &get_right() const;

    void set_statement_index(size_t statement_index) override;

    //TODO: Nodes like ++ should specify that only numerical types are allowed.
    [[nodiscard]] bool is_value_valid() const override;


    // FIXME: REMOVE THIS!!
    // Each node should set the type for itself.
    [[nodiscard]] type::type_ast_node *get_type() const override;


    void emit_constants(const compilation_unit_ast_node &root_node,
                        output::jvm_target::constant_pool &constant_pool) override;
};


}
