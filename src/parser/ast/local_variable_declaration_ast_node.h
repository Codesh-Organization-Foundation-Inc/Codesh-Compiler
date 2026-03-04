#pragma once

#include "parser/ast/impl/i_constant_pool_emitter.h"
#include "parser/ast/impl/i_symbolically_linked.h"
#include "parser/ast/type_declaration/attributes_ast_node.h"
#include "parser/ast/type_declaration/variable_declaration_ast_node.h"

#include <memory>

namespace codesh::semantic_analyzer
{
class local_variable_symbol;
class method_scope_symbol;
}


namespace codesh::ast
{

//TODO: Move to method operations namespace & directory
class local_variable_declaration_ast_node final : public type_decl::variable_declaration_ast_node,
    public impl::i_symbolically_linked<semantic_analyzer::local_variable_symbol>
{
    std::optional<std::reference_wrapper<semantic_analyzer::local_variable_symbol>> resolved_variable;

    size_t accessible_from;
    size_t accessible_to;

    // FOR IR:
    size_t bytecode_start_pc = 0;
    size_t bytecode_length = 0;

protected:
    [[nodiscard]] const std::optional<std::reference_wrapper<semantic_analyzer::local_variable_symbol>> &_get_resolved()
        const override;

public:
    explicit local_variable_declaration_ast_node(blasphemy::code_position code_position);

    void set_resolved(semantic_analyzer::local_variable_symbol &symbol) override;

    [[nodiscard]] size_t get_accessible_from() const;
    void set_accessible_from(size_t accessible_from);

    [[nodiscard]] size_t get_accessible_to() const;
    void set_accessible_to(size_t accessible_to);

    [[nodiscard]] size_t get_bytecode_start_pc() const;
    void set_bytecode_start_pc(size_t start_pc);

    [[nodiscard]] size_t get_bytecode_length() const;
    void set_bytecode_length(size_t length);

};

}
