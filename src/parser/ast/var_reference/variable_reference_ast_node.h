#pragma once

#include "defenition/fully_qualified_class_name.h"
#include "parser/ast/impl/i_constant_pool_emitter.h"
#include "parser/ast/impl/i_resolvable.h"
#include "parser/ast/local_variable_declaration_ast_node.h"
#include "parser/ast/var_reference/value_ast_node.h"

namespace codesh::semantic_analyzer
{
class variable_symbol;
}
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
        public codesh::ast::impl::i_symbolically_linked<codesh::semantic_analyzer::variable_symbol>,
        public codesh::ast::impl::i_constant_pool_emitter
{
    const codesh::definition::fully_qualified_class_name name;
    std::optional<std::reference_wrapper<codesh::semantic_analyzer::variable_symbol>> resolved_symbol;

    std::optional<std::reference_wrapper<const codesh::ast::local_variable_declaration_ast_node>> producing_declaration;

    std::optional<int> field_cpi;

protected:
    [[nodiscard]] const std::optional<std::reference_wrapper<codesh::semantic_analyzer::variable_symbol>> &
    _get_resolved() const override;

public:
    explicit variable_reference_ast_node(codesh::definition::fully_qualified_class_name name);
    /**
    * When a local_variable_declaration_ast_node is created and assigned on the spot, it will create an
    * assignment operator to later give it a value during runtime.
    *
    * In the AST, variable declaration (binding a name to a local slot) is modeled
    * separately from assignment (storing a value into that slot). On the JVM,
    * local variables correspond to fixed stack-frame slots determined at
    * compile-time, while assignments are emitted as runtime store instructions.
    *
    * Hence, for better compilation times, we can just cache the declaration producing this node and then resolve
    * it immediately after.
    */
    explicit variable_reference_ast_node(const codesh::ast::local_variable_declaration_ast_node &producing_declaration);

    void set_resolved(codesh::semantic_analyzer::variable_symbol &symbol) override;

    [[nodiscard]] codesh::ast::type::type_ast_node *get_type() const override;


    [[nodiscard]] const codesh::definition::fully_qualified_class_name &get_unresolved_name() const;

    [[nodiscard]] std::optional<std::reference_wrapper<const codesh::ast::local_variable_declaration_ast_node>>
        get_producing_declaration() const;


    void emit_constants(const codesh::ast::compilation_unit_ast_node &root_node,
                        codesh::output::jvm_target::constant_pool &constant_pool) override;

    void emit_ir(codesh::output::ir::code_block &containing_block,
                 const codesh::semantic_analyzer::symbol_table &symbol_table,
                 const codesh::ast::type_decl::type_declaration_ast_node &containing_type_decl) const override;
};
