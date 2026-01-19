#pragma once

#include "../../../operator/assignment/assign_operator_ast_node.h"
#include "../../../var_reference/value_ast_node.h"
#include "../method_operation_ast_node.h"

#include <memory>
#include <optional>

namespace codesh::ast::method
{
class method_scope_ast_node;
}

namespace codesh::ast::block
{

class for_ast_node : public method::operation::method_operation_ast_node
{
    std::optional<std::unique_ptr<op::assignment::assign_operator_ast_node>> init;
    std::optional<std::unique_ptr<var_reference::value_ast_node>> condition;
    std::optional<std::unique_ptr<impl::binary_ast_node>> iteration;

    method::method_scope_ast_node &body_scope;

public:
    for_ast_node(
        std::optional<std::unique_ptr<op::assignment::assign_operator_ast_node>> init,
        std::optional<std::unique_ptr<var_reference::value_ast_node>> condition,
        std::optional<std::unique_ptr<impl::binary_ast_node>> iteration,
        method::method_scope_ast_node &body_scope
    );

    [[nodiscard]] const std::optional<std::unique_ptr<op::assignment::assign_operator_ast_node>> &get_init() const;
    [[nodiscard]] const std::optional<std::unique_ptr<var_reference::value_ast_node>> &get_condition() const;
    [[nodiscard]] const std::optional<std::unique_ptr<impl::binary_ast_node>> &get_iteration() const;
    [[nodiscard]] const method::method_scope_ast_node &get_body_scope() const;

    void emit_ir(output::ir::code_block &containing_block,
                 const semantic_analyzer::symbol_table &symbol_table,
                 const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};

}
