#pragma once

#include "impl/instruction.h"

namespace codesh::output::ir
{

class new_instruction final : public instruction
{
    const int class_cp_index;
public:
    explicit new_instruction(int class_cp_index);

    [[nodiscard]] size_t size() const override;

    void emit(std::vector<instruction_container> &collector) const override;
};





}