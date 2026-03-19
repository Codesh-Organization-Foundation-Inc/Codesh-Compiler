#pragma once

#include "defenition/fully_qualified_name.h"
#include "parser/ast/impl/i_constant_pool_emitter.h"
#include "parser/ast/local_variable_declaration_ast_node.h"
#include "reference_association.h"
#include "value_ast_node.h"

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


namespace codesh::ast::var_reference
{
class variable_reference_ast_node : public value_ast_node,
        public impl::i_symbolically_linked<semantic_analyzer::variable_symbol>,
        public impl::i_constant_pool_emitter
{
    const definition::fully_qualified_name name;
    std::optional<std::reference_wrapper<semantic_analyzer::variable_symbol>> resolved_symbol;

    reference_association association;

    std::optional<std::reference_wrapper<const local_variable_declaration_ast_node>> producing_declaration;

    std::optional<int> field_cpi;

protected:
    [[nodiscard]] const std::optional<std::reference_wrapper<semantic_analyzer::variable_symbol>> &
    _get_resolved() const override;

public:
    variable_reference_ast_node(lexer::code_position code_position, definition::fully_qualified_name name);
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
    variable_reference_ast_node(lexer::code_position code_position,
            const local_variable_declaration_ast_node &producing_declaration);

    void set_resolved(semantic_analyzer::variable_symbol &symbol) override;

    [[nodiscard]] type::type_ast_node *get_type() const override;

    void set_association(reference_association association);
    [[nodiscard]] reference_association get_association() const;


    [[nodiscard]] const definition::fully_qualified_name &get_unresolved_name() const;

    [[nodiscard]] std::optional<std::reference_wrapper<const local_variable_declaration_ast_node>>
        get_producing_declaration() const;


    [[nodiscard]] std::optional<int> get_field_cpi() const;

    void emit_constants(const compilation_unit_ast_node &root_node,
                        output::jvm_target::constant_pool &constant_pool) override;

    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
                 const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};

}