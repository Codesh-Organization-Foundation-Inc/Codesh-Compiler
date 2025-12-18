#pragma once

#include "method_call_ast_node.h"

#include "../../../../defenition/fully_qualified_class_name.h"
#include "../../impl/i_resolvable.h"
#include "../../impl/ir_emitting_ast_node.h"
#include "../../var_reference/value_ast_node.h"

#include <memory>

namespace codesh::ast::method::operation
{

class method_call_ast_node : public impl::ir_emitting_ast_node, public impl::i_resolvable
{
    definition::fully_qualified_class_name name;
    std::optional<definition::fully_qualified_class_name> resolved_name;

    std::list<std::unique_ptr<var_reference::value_ast_node>> arguments;

protected:
    [[nodiscard]] std::optional<definition::fully_qualified_class_name> &_get_resolved_name() override;
    [[nodiscard]] const std::optional<definition::fully_qualified_class_name> &_get_resolved_name() const override;

public:
    [[nodiscard]] const definition::fully_qualified_class_name &get_name() const override;

    [[nodiscard]] definition::fully_qualified_class_name &get_fqcn();
    [[nodiscard]] const definition::fully_qualified_class_name &get_fqcn() const;


    [[nodiscard]] const std::list<std::unique_ptr<var_reference::value_ast_node>> &get_arguments() const;
    [[nodiscard]] std::list<std::unique_ptr<var_reference::value_ast_node>> &get_arguments();


    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
                 const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};

}
