#pragma once

#include "impl/typed_instruction.h"

namespace codesh::output::ir
{

class operator_instruction final : public typed_instruction
{
    operator_type op;

protected:
    [[nodiscard]] opcode first_generic() const override;

public:
    operator_instruction(instruction_type type, operator_type op);

    [[nodiscard]] size_t size() const override;

    void emit(std::vector<instruction_container> &collector) const override;
};

}
