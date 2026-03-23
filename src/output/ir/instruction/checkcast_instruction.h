#pragma once

#include "impl/instruction.h"

namespace codesh::output::ir
{

class checkcast_instruction final : public instruction
{
    const int class_cp_index;

public:
    explicit checkcast_instruction(int class_cp_index);

    [[nodiscard]] size_t size() const override;

    void emit(std::vector<instruction_container> &collector) const override;
};

}
