#pragma once

#include "variable_declaration_ast_node.h"
#include "parser/ast/impl/i_descriptor_emitter.h"
#include "parser/ast/impl/i_resolvable.h"
#include "parser/ast/var_reference/value_ast_node.h"

#include <optional>

namespace codesh::ast::type_decl
{
class type_declaration_ast_node;

class field_declaration_ast_node : public variable_declaration_ast_node, public impl::i_descriptor_emitter
{
    std::unique_ptr<var_reference::value_ast_node> value;

    std::optional<std::reference_wrapper<semantic_analyzer::field_symbol>> resolved_symbol;

    type_declaration_ast_node *containing_type = nullptr;
    std::optional<int> field_cpi;

public:
    explicit field_declaration_ast_node(lexer::code_position pos);

    [[nodiscard]] var_reference::value_ast_node* get_value() const;
    void set_value(std::unique_ptr<var_reference::value_ast_node> val);

    void set_containing_type(type_declaration_ast_node &type);
    [[nodiscard]] std::optional<int> get_field_cpi() const;

    void emit_constants(const compilation_unit_ast_node &root_node,
                        output::jvm_target::constant_pool &constant_pool) override;

    [[nodiscard]] std::string generate_descriptor(bool resolved) const override;

};
}
