#pragma once

#include "impl/simple_instruction.h"
#include "impl/typed_instruction.h"

namespace codesh::output::ir
{

class return_instruction final : public simple_instruction
{
public:
    /**
     * Void return
     */
    return_instruction();
    explicit return_instruction(instruction_type type);
};

}
