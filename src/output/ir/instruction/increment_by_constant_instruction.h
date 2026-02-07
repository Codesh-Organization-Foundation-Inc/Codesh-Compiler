#pragma once

#include "impl/instruction.h"

#include <optional>

namespace codesh::output::ir
{
enum class instruction_type;

class increment_by_constant_instruction final : public instruction
{
    instruction_type type;

    const int target_lvt_index;
    //FIXME: Int only
    const int constant_addition;

    const std::optional<int> constant_addition_cpi;

public:
    increment_by_constant_instruction(instruction_type type, int target_lvt_index, int constant_addition,
            std::optional<int> constant_addition_cpi);

    [[nodiscard]] size_t size() const override;

    void emit(std::vector<instruction_container> &collector) const override;
};

}
