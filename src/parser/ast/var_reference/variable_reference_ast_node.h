#pragma once

#include "../../../defenition/fully_qualified_class_name.h"
#include "../impl/i_constant_pool_emitter.h"
#include "../impl/i_resolvable.h"
#include "value_ast_node.h"

namespace codesh::ast
{
class compilation_unit_ast_node;
}
namespace codesh::semantic_analyzer
{
class field_symbol;
class type_symbol;
}

class variable_reference_ast_node : public codesh::ast::var_reference::value_ast_node,
        //TODO: Extend to all variables, not just fields.
        public codesh::ast::impl::i_resolvable<codesh::semantic_analyzer::field_symbol>,
        public codesh::ast::impl::i_constant_pool_emitter
{
    const codesh::definition::fully_qualified_class_name name;
    std::optional<std::reference_wrapper<codesh::semantic_analyzer::field_symbol>> resolved_symbol;

protected:
    [[nodiscard]] const std::optional<std::reference_wrapper<codesh::semantic_analyzer::field_symbol>> &
    _get_resolved() const override;

public:
    explicit variable_reference_ast_node(codesh::definition::fully_qualified_class_name name);

    [[nodiscard]] const codesh::definition::fully_qualified_class_name &get_unresolved_name() const override;
    void set_resolved(codesh::semantic_analyzer::field_symbol &symbol) override;

    [[nodiscard]] codesh::ast::type::type_ast_node *get_type() const override;


    void emit_constants(const codesh::ast::compilation_unit_ast_node &root_node,
                        codesh::output::jvm_target::constant_pool &constant_pool) const override;

    void emit_ir(codesh::output::ir::code_block &containing_block,
                 const codesh::semantic_analyzer::symbol_table &symbol_table,
                 const codesh::ast::type_decl::type_declaration_ast_node &containing_type_decl) const override;
};
