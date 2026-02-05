#include "new_ast_node.h"


#include "semantic_analyzer/symbol_table/symbol_table.h"

codesh::ast::op::new_ast_node::new_ast_node(const blasphemy::code_position code_position,
        std::unique_ptr<type::custom_type_ast_node> constructed_type) :
    method_call_ast_node(code_position), constructed_type(std::move(constructed_type))
{
    // Constructor name of new is always <init>
    get_fqcn().add("<init>");
}

codesh::ast::type::custom_type_ast_node &codesh::ast::op::new_ast_node::get_constructed_type()
{
    return *constructed_type;
}

const codesh::ast::type::custom_type_ast_node &codesh::ast::op::new_ast_node::get_constructed_type() const
{
    return *constructed_type;
}

void codesh::ast::op::new_ast_node::emit_ir(output::ir::code_block &containing_block,
        const semantic_analyzer::symbol_table &symbol_table,
        const type_decl::type_declaration_ast_node &containing_type_decl) const
{
}
