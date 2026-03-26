#include "array_length_instruction.h"

codesh::output::ir::array_length_instruction::array_length_instruction() :
    simple_instruction(opcode::ARRAY_LENGTH, 0, 1)
{
}
