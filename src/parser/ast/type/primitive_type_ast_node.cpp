#include "primitive_type_ast_node.h"

#include <sstream>
#include <stdexcept>

codesh::ast::type::primitive_type_ast_node::primitive_type_ast_node(const definition::primitive_type type) : type(type)
{
}

std::string codesh::ast::type::primitive_type_ast_node::generate_descriptor() const
{
    std::ostringstream result;

    for (int i = 0; i < this->get_array_dimensions(); i++)
    {
        result << '[';
    }

    switch (get_type())
    {
    case definition::primitive_type::VOID: result << 'V'; break;
    case definition::primitive_type::FLOAT: result << 'F'; break;
    case definition::primitive_type::CHAR: result << 'C'; break;
    case definition::primitive_type::DOUBLE: result << 'D'; break;
    case definition::primitive_type::INTEGER: result << 'I'; break;
    case definition::primitive_type::LONG: result << 'L'; break;
    case definition::primitive_type::SHORT: result << 'S'; break;
    case definition::primitive_type::BYTE: result << 'B'; break;
    case definition::primitive_type::BOOLEAN: result << 'Z'; break;

    default: throw std::runtime_error("Unknown primitive type");
    }

    return result.str();
}


codesh::definition::primitive_type codesh::ast::type::primitive_type_ast_node::get_type() const
{
    return type;
}
