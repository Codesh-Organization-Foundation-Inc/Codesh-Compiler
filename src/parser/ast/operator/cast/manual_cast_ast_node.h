#pragma once

#include "cast_ast_node.h"
#include "parser/ast/type/type_ast_node.h"

#include <memory>

namespace codesh::ast::op::assignment
{

class manual_cast_ast_node final : public cast_ast_node
{
    std::unique_ptr<type::type_ast_node> target_type;

public:
    manual_cast_ast_node(lexer::code_position code_position, std::unique_ptr<value_ast_node> value,
        std::unique_ptr<type::type_ast_node> target_type);

    [[nodiscard]] type::type_ast_node *get_type() const override;

    void emit_constants(const compilation_unit_ast_node &root_node,
                        output::jvm_target::constant_pool &constant_pool) override;
    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
        const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};

}
