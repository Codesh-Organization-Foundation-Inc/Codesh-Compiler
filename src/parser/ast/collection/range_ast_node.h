#pragma once

#include "parser/ast/type/custom_type_ast_node.h"
#include "collection_ast_node.h"

namespace codesh::ast::collection
{

class range_ast_node : public collection_ast_node
{
    std::unique_ptr<value_ast_node> from;
    std::unique_ptr<value_ast_node> to;
    std::unique_ptr<value_ast_node> skip;

    const std::unique_ptr<type::custom_type_ast_node> type;

public:
    range_ast_node(blasphemy::code_position code_position, std::unique_ptr<value_ast_node> from, std::unique_ptr<value_ast_node> to,
            std::unique_ptr<value_ast_node> skip);

    [[nodiscard]] value_ast_node &get_from() const;
    [[nodiscard]] value_ast_node &get_to() const;
    [[nodiscard]] value_ast_node &get_skip() const;

    void set_statement_index(size_t statement_index) override;

    [[nodiscard]] type::type_ast_node *get_type() const override;

    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
                 const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};

}
