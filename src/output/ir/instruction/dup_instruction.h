#pragma once

#include "impl/simple_instruction.h"

namespace codesh::output::ir
{

class dup_instruction final : public simple_instruction
{
public:
    dup_instruction();
};

}
