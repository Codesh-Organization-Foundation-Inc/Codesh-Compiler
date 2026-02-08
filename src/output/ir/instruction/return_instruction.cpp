#include "return_instruction.h"

codesh::output::ir::return_instruction::return_instruction() :
    simple_instruction(opcode::RETURN, 0, 1)
{
}

codesh::output::ir::return_instruction::return_instruction(const instruction_type type) :
    simple_instruction(static_cast<opcode>(*opcode::I_RETURN + *type), -1, 1)
{
}
