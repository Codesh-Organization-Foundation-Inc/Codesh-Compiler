#pragma once

#include "parser/ast/impl/ast_node.h"
#include "parser/ast/impl/i_constant_pool_emitter.h"
#include "parser/ast/impl/i_descriptor_emitter.h"
#include "parser/ast/impl/i_resolvable.h"
#include "parser/ast/var_reference/value_ast_node.h"


namespace codesh::ast::type_decl
{
class field_declaration_ast_node : public impl::ast_node, public impl::i_descriptor_emitter,
        public impl::i_constant_pool_emitter
{
    definition::fully_qualified_name name;
    std::unique_ptr<type::type_ast_node> type;
    std::unique_ptr<attributes_ast_node> attributes;
    std::unique_ptr<var_reference::value_ast_node> value;

    std::optional<std::reference_wrapper<semantic_analyzer::field_symbol>> resolved_symbol;

public:
    field_declaration_ast_node(blasphemy::code_position pos, definition::fully_qualified_name name,
            std::unique_ptr<type::type_ast_node> type);

    [[nodiscard]] const definition::fully_qualified_name& get_name() const;
    [[nodiscard]] type::type_ast_node* get_type() const;

    [[nodiscard]] attributes_ast_node *get_attributes() const;
    void set_attributes(std::unique_ptr<attributes_ast_node> attributes);

    [[nodiscard]] var_reference::value_ast_node* get_value() const;
    void set_value(std::unique_ptr<var_reference::value_ast_node> val);

    [[nodiscard]] std::string generate_descriptor(bool resolved) const override;

    void emit_constants(const compilation_unit_ast_node& root,output::jvm_target::constant_pool& constant_pool) override;
};
}
