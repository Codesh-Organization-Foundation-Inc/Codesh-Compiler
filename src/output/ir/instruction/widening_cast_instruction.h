#pragma once

#include "impl/simple_instruction.h"
#include "impl/typed_instruction.h"

namespace codesh::output::ir
{

class widening_cast_instruction final : public simple_instruction
{
    static opcode lookup_opcode(instruction_type from, instruction_type to);
    static int lookup_delta(instruction_type from, instruction_type to);

public:
    widening_cast_instruction(instruction_type from, instruction_type to);
};

}
