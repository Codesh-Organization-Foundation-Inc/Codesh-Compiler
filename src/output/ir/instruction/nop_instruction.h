#pragma once

#include "impl/simple_instruction.h"

namespace codesh::output::ir
{

class nop_instruction final : public simple_instruction
{
public:
    nop_instruction();
};

}
