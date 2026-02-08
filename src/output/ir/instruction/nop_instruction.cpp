#include "nop_instruction.h"

codesh::output::ir::nop_instruction::nop_instruction() : simple_instruction(opcode::NOP, 0, 1)
{
}
