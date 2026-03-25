#pragma once

#include "impl/simple_instruction.h"

namespace codesh::output::ir
{

class aconst_null_instruction final : public simple_instruction
{
public:
    aconst_null_instruction();
};

}
