#pragma once

#include "impl/typed_instruction.h"

namespace codesh::output::ir
{

class load_instruction final : public typed_instruction
{
protected:
    [[nodiscard]] opcode first_generic() const override;

public:
    explicit load_instruction(instruction_type type, unsigned char local_var_index);
};

}
