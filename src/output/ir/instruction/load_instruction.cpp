#include "load_instruction.h"

codesh::output::ir::opcode codesh::output::ir::load_instruction::first_generic() const
{
    return opcode::I_LOAD;
}

codesh::output::ir::load_instruction::load_instruction(const instruction_type type, const unsigned char local_var_index) :
    typed_instruction(type, local_var_index)
{
}
