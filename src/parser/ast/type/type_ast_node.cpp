#include "type_ast_node.h"

#include "custom_type_ast_node.h"
#include "output/ir/instruction/impl/typed_instruction.h"
#include "primitive_type_ast_node.h"

codesh::ast::type::type_ast_node::type_ast_node(const lexer::code_position code_position) :
    ast_node(code_position),
    array_dimensions(0)
{
}

std::unique_ptr<codesh::ast::type::type_ast_node> codesh::ast::type::type_ast_node::from_descriptor(
        const std::string &descriptor, size_t &pos, const lexer::code_position code_position)
{
    int array_dimensions = 0;
    while (pos < descriptor.size() && descriptor[pos] == '[')
    {
        array_dimensions++;
        pos++;
    }

    if (pos >= descriptor.size())
    {
        throw std::runtime_error("Unexpected end of descriptor");
    }


    const auto type_letter = descriptor.at(pos++);

    std::unique_ptr<type_ast_node> result;
    switch (type_letter)
    {
    case 'B':
        result = std::make_unique<primitive_type_ast_node>(code_position, definition::primitive_type::BYTE);
        break;
    case 'C':
        result = std::make_unique<primitive_type_ast_node>(code_position, definition::primitive_type::CHAR);
        break;
    case 'D':
        result = std::make_unique<primitive_type_ast_node>(code_position, definition::primitive_type::DOUBLE);
        break;
    case 'F':
        result = std::make_unique<primitive_type_ast_node>(code_position, definition::primitive_type::FLOAT);
        break;
    case 'I':
        result = std::make_unique<primitive_type_ast_node>(code_position, definition::primitive_type::INTEGER);
        break;
    case 'J':
        result = std::make_unique<primitive_type_ast_node>(code_position, definition::primitive_type::LONG);
        break;
    case 'S':
        result = std::make_unique<primitive_type_ast_node>(code_position, definition::primitive_type::SHORT);
        break;
    case 'Z':
        result = std::make_unique<primitive_type_ast_node>(code_position, definition::primitive_type::BOOLEAN);
        break;
    case 'V':
        result = std::make_unique<primitive_type_ast_node>(code_position, definition::primitive_type::VOID);
        break;

    case 'L': {
        const size_t end = descriptor.find(';', pos);
        if (end == std::string::npos)
            throw std::runtime_error("Invalid descriptor: missing ';': " + descriptor);

        const std::string class_name = descriptor.substr(pos, end - pos);
        pos = end + 1;

        result = std::make_unique<custom_type_ast_node>(
            code_position,
            definition::fully_qualified_name::parse(class_name, code_position)
        );
        break;
    }

    default:
        throw std::runtime_error("Unknown descriptor char: " + std::string(1, type_letter));
    }

    result->set_array_dimensions(array_dimensions);
    return result;
}

int codesh::ast::type::type_ast_node::get_array_dimensions() const
{
    return array_dimensions;
}

void codesh::ast::type::type_ast_node::set_array_dimensions(const int array_dimensions)
{
    this->array_dimensions = array_dimensions;
}

std::unique_ptr<codesh::ast::type::type_ast_node> codesh::ast::type::type_ast_node::clone() const
{
    return std::unique_ptr<type_ast_node>(_clone());
}

codesh::output::ir::instruction_type codesh::ast::type::type_ast_node::to_instruction_type() const
{
    if (array_dimensions > 0)
        return output::ir::instruction_type::REFERENCE;

    return _to_instruction_type();
}
