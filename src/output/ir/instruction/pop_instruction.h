#pragma once

#include "impl/simple_instruction.h"

namespace codesh::output::ir
{

class pop_instruction final : public simple_instruction
{
public:
    pop_instruction();
};

}
