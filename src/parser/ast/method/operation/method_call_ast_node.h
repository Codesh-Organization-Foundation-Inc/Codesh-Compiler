#pragma once

#include "method_call_ast_node.h"
#include "method_operation_ast_node.h"

#include "../../../../defenition/fully_qualified_class_name.h"
#include "../../impl/i_constant_pool_emitter.h"
#include "../../impl/i_ir_emitter.h"
#include "../../impl/i_resolvable.h"
#include "../../var_reference/value_ast_node.h"

#include <memory>
#include <deque>

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

class method_call_ast_node : public method_operation_ast_node,
    public impl::i_constant_pool_emitter,
    public impl::i_resolvable<semantic_analyzer::method_symbol>,
    public impl::i_descriptor_emitter
{
    definition::fully_qualified_class_name name;
    std::optional<std::reference_wrapper<semantic_analyzer::method_symbol>> resolved_symbol;

    std::deque<std::unique_ptr<var_reference::value_ast_node>> arguments;

protected:
    [[nodiscard]] const std::optional<std::reference_wrapper<semantic_analyzer::method_symbol>> &_get_resolved() const
        override;

public:
    void set_resolved(semantic_analyzer::method_symbol &symbol) override;

    [[nodiscard]] const definition::fully_qualified_class_name &get_unresolved_name() const override;

    [[nodiscard]] definition::fully_qualified_class_name &get_fqcn();
    [[nodiscard]] const definition::fully_qualified_class_name &get_fqcn() const;

    using i_descriptor_emitter::generate_descriptor;
    [[nodiscard]] std::string generate_descriptor(bool resolved) const override;


    [[nodiscard]] const std::deque<std::unique_ptr<var_reference::value_ast_node>> &get_arguments() const;
    [[nodiscard]] std::deque<std::unique_ptr<var_reference::value_ast_node>> &get_arguments();

    void emit_constants(const compilation_unit_ast_node &root_node,
                        output::jvm_target::constant_pool &constant_pool) override;

    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
                 const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};

}
