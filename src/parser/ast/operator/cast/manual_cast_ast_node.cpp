#include "manual_cast_ast_node.h"

codesh::ast::op::assignment::manual_cast_ast_node::manual_cast_ast_node(
        const lexer::code_position code_position, std::unique_ptr<value_ast_node> value,
        std::unique_ptr<type::type_ast_node> target_type) :
    cast_ast_node(code_position, std::move(value)),
    target_type(std::move(target_type))
{
}

codesh::ast::type::type_ast_node *codesh::ast::op::assignment::manual_cast_ast_node::get_type() const
{
    return target_type.get();
}

void codesh::ast::op::assignment::manual_cast_ast_node::emit_constants(const compilation_unit_ast_node &root_node,
                                                                       output::jvm_target::constant_pool &constant_pool)
{
}

void codesh::ast::op::assignment::manual_cast_ast_node::emit_ir(output::ir::code_block &containing_block,
    const semantic_analyzer::symbol_table &symbol_table,
    const type_decl::type_declaration_ast_node &containing_type_decl) const
{
}
