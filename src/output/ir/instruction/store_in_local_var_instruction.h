#pragma once

#include "impl/typed_instruction.h"

namespace codesh::output::ir
{

class store_in_local_var_instruction final : public typed_instruction
{
protected:
    [[nodiscard]] opcode first_generic() const override;

public:
    store_in_local_var_instruction(instruction_type type, int local_var_index);
};

}
