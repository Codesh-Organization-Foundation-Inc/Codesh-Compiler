#include "null_type_ast_node.h"

#include "output/ir/instruction/impl/typed_instruction.h"

codesh::ast::type::null_type_ast_node::null_type_ast_node(const lexer::code_position code_position) :
    type_ast_node(code_position)
{
}

std::string codesh::ast::type::null_type_ast_node::generate_descriptor(bool) const
{
    return "null";
}

codesh::output::ir::instruction_type codesh::ast::type::null_type_ast_node::_to_instruction_type() const
{
    return output::ir::instruction_type::REFERENCE;
}

codesh::ast::type::type_ast_node *codesh::ast::type::null_type_ast_node::_clone() const
{
    return new null_type_ast_node(*this);
}

std::string codesh::ast::type::null_type_ast_node::to_pretty_string() const
{
    return "null";
}
