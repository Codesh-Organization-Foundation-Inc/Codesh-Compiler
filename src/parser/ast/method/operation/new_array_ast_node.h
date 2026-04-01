#pragma once

#include "parser/ast/impl/ast_node.h"
#include "parser/ast/type/type_ast_node.h"
#include "parser/ast/var_reference/value_ast_node.h"

#include <memory>
#include <vector>

namespace codesh::ast::op
{

class new_array_ast_node final : public var_reference::value_ast_node
{
    std::unique_ptr<type::type_ast_node> element_type;
    std::vector<std::unique_ptr<value_ast_node>> dimensions;

public:
    new_array_ast_node(lexer::code_position code_position, std::unique_ptr<type::type_ast_node> element_type);

    void add_dimension(std::unique_ptr<value_ast_node> size_expr);

    [[nodiscard]] type::type_ast_node &get_element_type() const;

    [[nodiscard]] const std::vector<std::unique_ptr<value_ast_node>> &get_dimensions() const;

    [[nodiscard]] type::type_ast_node *get_type() const override;

    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
            const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};

}