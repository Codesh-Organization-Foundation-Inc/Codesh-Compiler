#include "new_ast_node.h"

#include "output/ir/code_block.h"
#include "semantic_analyzer/symbol_table/symbol_table.h"
#include "parser/ast/type/type_ast_node.h"
#include "parser/ast/var_reference/value_ast_node.h"


codesh::ast::op::new_ast_node::new_ast_node(
        blasphemy::code_position code_position, std::unique_ptr<type::type_ast_node> constructed_type) :
    value_ast_node(code_position),
    constructed_type(std::move(constructed_type))
{
}

codesh::ast::type::type_ast_node &
codesh::ast::op::new_ast_node::get_constructed_type()
{
    return *constructed_type;
}

const codesh::ast::type::type_ast_node &
codesh::ast::op::new_ast_node::get_constructed_type() const
{
    return *constructed_type;
}

std::vector<std::unique_ptr<codesh::ast::var_reference::value_ast_node>> &
codesh::ast::op::new_ast_node::get_arguments()
{
    return arguments;
}

void codesh::ast::op::new_ast_node::emit_ir(
    output::ir::code_block &containing_block,
    const semantic_analyzer::symbol_table &symbol_table,
    const type_decl::type_declaration_ast_node &containing_type_decl
) const
{
}
