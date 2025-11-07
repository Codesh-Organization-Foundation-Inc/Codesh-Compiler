#include "primitive_type_ast_node.h"

#include <stdexcept>

codesh::ast::type::primitive_type_ast_node::primitive_type_ast_node(const definition::primitive_type type) : type(type)
{
}

std::string codesh::ast::type::primitive_type_ast_node::generate_descriptor() const
{
    switch (get_type())
    {
    case definition::primitive_type::VOID: return "V";
    case definition::primitive_type::FLOAT: return "F";
    case definition::primitive_type::CHAR: return "C";
    case definition::primitive_type::DOUBLE: return "D";
    case definition::primitive_type::INTEGER: return "I";
    case definition::primitive_type::LONG: return "L";
    case definition::primitive_type::SHORT: return "S";
    case definition::primitive_type::BYTE: return "B";
    case definition::primitive_type::BOOLEAN: return "Z";

    default: throw std::runtime_error("Unknown primitive type");
    }
}


codesh::definition::primitive_type codesh::ast::type::primitive_type_ast_node::get_type() const
{
    return type;
}
