#pragma once
#include "../method_operation_ast_node.h"
#include "../../../var_reference/value_ast_node.h"


#include <memory>
#include <optional>


namespace codesh::ast::method
{
class method_scope_ast_node;
}
namespace codesh::ast::block // TODO: do it like - block::logic
{

class if_ast_node : public method::operation::method_operation_ast_node
{
    std::unique_ptr<var_reference::value_ast_node> condition;
    method::method_scope_ast_node &if_scope;
    std::optional<std::reference_wrapper<method::method_scope_ast_node>> else_scope;


public:
    explicit if_ast_node(
        std::unique_ptr<var_reference::value_ast_node> condition,
        method::method_scope_ast_node &if_scope,
        std::optional<std::reference_wrapper<method::method_scope_ast_node>> else_scope = std::nullopt
    );

    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
                 const type_decl::type_declaration_ast_node &containing_type_decl) const override;

    [[nodiscard]] const var_reference::value_ast_node &get_condition() const;
    [[nodiscard]] method::method_scope_ast_node &get_if_scope() const;
    [[nodiscard]] std::optional<std::reference_wrapper<method::method_scope_ast_node>> get_else_scope() const;

    void set_condition(std::unique_ptr<var_reference::value_ast_node> condition);
    void set_else_scope(method::method_scope_ast_node& scope);
    void clear_else_scope();
};
}