#pragma once

#include "impl/instruction.h"

#include <optional>

namespace codesh::output::ir
{
class increment_int_by_constant_instruction final : public instruction
{
    const int target_lvt_index;
    const int constant_addition;

    const std::optional<int> constant_addition_cpi;

public:
    increment_int_by_constant_instruction(int target_lvt_index, int constant_addition,
            std::optional<int> constant_addition_cpi);

    [[nodiscard]] size_t size() const override;

    void emit(std::vector<instruction_container> &collector) const override;
};

}
