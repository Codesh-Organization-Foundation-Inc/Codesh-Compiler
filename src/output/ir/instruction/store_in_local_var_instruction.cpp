#include "store_in_local_var_instruction.h"

codesh::output::ir::opcode codesh::output::ir::store_in_local_var_instruction::first_generic() const
{
    return opcode::I_STORE;
}

codesh::output::ir::store_in_local_var_instruction::store_in_local_var_instruction(const instruction_type type,
                                                                                   const int local_var_index) :
    typed_instruction(type, local_var_index)
{
}
