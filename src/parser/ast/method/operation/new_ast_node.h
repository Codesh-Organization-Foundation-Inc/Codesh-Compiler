#pragma once

#include "method_call_ast_node.h"
#include "parser/ast/type/custom_type_ast_node.h"
#include "parser/ast/type/type_ast_node.h"

namespace codesh::ast::op
{

class new_ast_node final : public method::operation::method_call_ast_node
{
    std::unique_ptr<type::custom_type_ast_node> constructed_type;

public:
    new_ast_node(blasphemy::code_position code_position, std::unique_ptr<type::custom_type_ast_node> constructed_type);

    [[nodiscard]] type::custom_type_ast_node &get_constructed_type() const;

    [[nodiscard]] std::string to_pretty_string() const override;

    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
            const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};

}
