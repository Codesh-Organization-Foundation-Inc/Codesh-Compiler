#pragma once

#include "parser/ast/impl/unary_ast_node.h"
#include "parser/ast/type/primitive_type_ast_node.h"

#include <memory>

namespace codesh::ast::method::operation
{

class array_length_ast_node final : public impl::unary_ast_node
{
    // The process of getting an array's length always produces an integer:
    std::unique_ptr<type::primitive_type_ast_node> integer_type;

public:
    array_length_ast_node(lexer::code_position code_position, std::unique_ptr<value_ast_node> child);

    [[nodiscard]] std::string to_pretty_string() const override;

    [[nodiscard]] type::type_ast_node *get_type() const override;

    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
                 const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};

}
