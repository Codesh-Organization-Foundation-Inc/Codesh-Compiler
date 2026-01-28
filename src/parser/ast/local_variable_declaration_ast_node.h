#pragma once

#include "parser/ast/impl/i_constant_pool_emitter.h"
#include "parser/ast/impl/i_symbolically_linked.h"
#include "parser/ast/type/type_ast_node.h"
#include "parser/ast/type_declaration/attributes_ast_node.h"
#include "parser/ast/var_reference/value_ast_node.h"

#include <memory>
#include <string>

namespace codesh::semantic_analyzer
{
class local_variable_symbol;
class method_scope_symbol;
}


namespace codesh::ast
{

//TODO: Move to method operations namespace & directory
class local_variable_declaration_ast_node : public impl::i_constant_pool_emitter,
        public impl::i_symbolically_linked<semantic_analyzer::local_variable_symbol>
{
    std::optional<std::reference_wrapper<semantic_analyzer::local_variable_symbol>> resolved_variable;

    std::string name;
    std::unique_ptr<type::type_ast_node> type;
    
    std::unique_ptr<type_decl::attributes_ast_node> attributes;

    size_t accessible_from;
    size_t accessible_to;

    // Bytecode positions - set during IR emission finalization
    mutable size_t bytecode_start_pc = 0;
    mutable size_t bytecode_length = 0;

protected:
    [[nodiscard]] const std::optional<std::reference_wrapper<semantic_analyzer::local_variable_symbol>> &_get_resolved()
        const override;

public:
    local_variable_declaration_ast_node();

    void set_resolved(semantic_analyzer::local_variable_symbol &symbol) override;


    [[nodiscard]] std::string get_name() const;
    void set_name(const std::string &name);

    [[nodiscard]] type::type_ast_node *get_type() const;
    void set_type(std::unique_ptr<type::type_ast_node> type);

    [[nodiscard]] type_decl::attributes_ast_node *get_attributes() const;
    void set_attributes(std::unique_ptr<type_decl::attributes_ast_node> attributes);

    [[nodiscard]] size_t get_accessible_from() const;
    void set_accessible_from(size_t accessible_from);

    [[nodiscard]] size_t get_accessible_to() const;
    void set_accessible_to(size_t accessible_to);

    [[nodiscard]] size_t get_bytecode_start_pc() const;
    void set_bytecode_start_pc(size_t start_pc) const;

    [[nodiscard]] size_t get_bytecode_length() const;
    void set_bytecode_length(size_t length) const;


    void emit_constants(const compilation_unit_ast_node &root_node,
                output::jvm_target::constant_pool &constant_pool) override;
};

}
