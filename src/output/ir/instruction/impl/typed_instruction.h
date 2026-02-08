#pragma once

#include "instruction.h"
#include "opcode.h"

namespace codesh::output::ir
{

class typed_instruction : public instruction
{
    static constexpr size_t CONSTANT_INDEXES_COUNT = 4;

    const unsigned char index;

protected:
    instruction_type type;
    [[nodiscard]] virtual opcode first_generic() const = 0;

public:
    typed_instruction(instruction_type type, unsigned char index);

    [[nodiscard]] size_t size() const override;

    void emit(std::vector<instruction_container> &collector) const override;
};

}
