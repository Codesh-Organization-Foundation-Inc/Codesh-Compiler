#pragma once

#include "method_call_ast_node.h"

#include "../../../../defenition/fully_qualified_class_name.h"
#include "../../impl/i_resolvable.h"
#include "../../impl/ir_emitting_ast_node.h"
#include "../../var_reference/value_ast_node.h"

#include <memory>
#include <vector>

namespace codesh::semantic_analyzer
{
class method_symbol;
}

namespace codesh::ast::method::operation
{

class method_call_ast_node : public impl::ir_emitting_ast_node, public impl::i_resolvable,
    public impl::i_descriptor_emitter
{
    definition::fully_qualified_class_name name;
    std::optional<definition::fully_qualified_class_name> resolved_name;

    std::optional<std::reference_wrapper<semantic_analyzer::method_symbol>> referred_method;

    std::vector<std::unique_ptr<var_reference::value_ast_node>> arguments;

protected:
    [[nodiscard]] std::optional<definition::fully_qualified_class_name> &_get_resolved_name() override;
    [[nodiscard]] const std::optional<definition::fully_qualified_class_name> &_get_resolved_name() const override;

public:
    [[nodiscard]] const definition::fully_qualified_class_name &get_name() const override;

    [[nodiscard]] definition::fully_qualified_class_name &get_fqcn();
    [[nodiscard]] const definition::fully_qualified_class_name &get_fqcn() const;

    [[nodiscard]] semantic_analyzer::method_symbol &get_referred_method() const;
    void set_referred_method(semantic_analyzer::method_symbol &method);

    using i_descriptor_emitter::generate_descriptor;
    [[nodiscard]] std::string generate_descriptor(bool resolved) const override;


    [[nodiscard]] const std::vector<std::unique_ptr<var_reference::value_ast_node>> &get_arguments() const;
    [[nodiscard]] std::vector<std::unique_ptr<var_reference::value_ast_node>> &get_arguments();


    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
                 const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};

}
