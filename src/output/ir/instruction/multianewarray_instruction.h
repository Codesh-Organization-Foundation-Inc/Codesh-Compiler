#pragma once

#include "impl/instruction.h"

namespace codesh::output::ir
{

class multianewarray_instruction final : public instruction
{
    const int class_cp_index;
    const unsigned char dimensions;

public:
    multianewarray_instruction(int class_cp_index, unsigned char dimensions);

    [[nodiscard]] size_t size() const override;

    void emit(std::vector<instruction_container> &collector) const override;
};

}
