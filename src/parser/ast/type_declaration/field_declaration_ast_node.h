#pragma once

#include "variable_declaration_ast_node.h"
#include "parser/ast/impl/ast_node.h"
#include "parser/ast/impl/i_constant_pool_emitter.h"
#include "parser/ast/impl/i_descriptor_emitter.h"
#include "parser/ast/impl/i_resolvable.h"
#include "parser/ast/var_reference/value_ast_node.h"


namespace codesh::ast::type_decl
{
class field_declaration_ast_node : public variable_declaration_ast_node, public impl::i_descriptor_emitter
{
    std::unique_ptr<var_reference::value_ast_node> value;

    std::optional<std::reference_wrapper<semantic_analyzer::field_symbol>> resolved_symbol;

public:
    explicit field_declaration_ast_node(blasphemy::code_position pos);

    [[nodiscard]] var_reference::value_ast_node* get_value() const;
    void set_value(std::unique_ptr<var_reference::value_ast_node> val);

    [[nodiscard]] std::string generate_descriptor(bool resolved) const override;

};
}
