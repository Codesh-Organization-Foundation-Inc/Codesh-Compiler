#pragma once

#include "impl/simple_instruction.h"

namespace codesh::output::ir
{

class array_length_instruction final : public simple_instruction
{
public:
    array_length_instruction();
};

}
