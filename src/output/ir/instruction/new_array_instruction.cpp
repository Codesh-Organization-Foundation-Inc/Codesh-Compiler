#include "new_array_instruction.h"

#include "impl/instruction_container.h"
#include "impl/opcode.h"

#include <stdexcept>

codesh::output::ir::new_array_instruction::new_array_instruction(const definition::primitive_type type) :
    type(type)
{
}

size_t codesh::output::ir::new_array_instruction::size() const
{
    return 2;
}

void codesh::output::ir::new_array_instruction::emit(std::vector<instruction_container> &collector) const
{
    unsigned char type_code;
    switch (type)
    {
    case definition::primitive_type::BOOLEAN: type_code = 4; break;
    case definition::primitive_type::CHAR: type_code = 5; break;
    case definition::primitive_type::FLOAT: type_code = 6; break;
    case definition::primitive_type::DOUBLE: type_code = 7; break;
    case definition::primitive_type::BYTE: type_code = 8; break;
    case definition::primitive_type::SHORT: type_code = 9; break;
    case definition::primitive_type::INTEGER: type_code = 10; break;
    case definition::primitive_type::LONG: type_code = 11; break;
    default: throw std::runtime_error("Invalid primitive type for new array");
    }

    collector.emplace_back(
        std::vector{
            *opcode::NEW_ARRAY,
            type_code
        },
        0
    );
}
