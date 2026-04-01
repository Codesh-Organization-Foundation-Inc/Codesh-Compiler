#include "new_array_ast_node.h"

using namespace codesh::ast::op;

new_array_ast_node::new_array_ast_node(const lexer::code_position code_position,
        std::unique_ptr<type::type_ast_node> element_type) :
    value_ast_node(code_position),
    element_type(std::move(element_type))
{
}

void new_array_ast_node::add_dimension(std::unique_ptr<value_ast_node> size_expr)
{
    dimensions.push_back(std::move(size_expr));
    element_type->set_array_dimensions(static_cast<int>(dimensions.size()));
}

codesh::ast::type::type_ast_node &new_array_ast_node::get_element_type() const
{
    return *element_type;
}

const std::vector<std::unique_ptr<codesh::ast::var_reference::value_ast_node>> &new_array_ast_node::get_dimensions()
    const
{
    return dimensions;
}


codesh::ast::type::type_ast_node *new_array_ast_node::get_type() const
{
    return element_type.get();
}


void new_array_ast_node::emit_ir(output::ir::code_block &containing_block,
        const semantic_analyzer::symbol_table &symbol_table,
        const type_decl::type_declaration_ast_node &containing_type_decl) const
{
    // TODO: Implement
}
