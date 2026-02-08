#pragma once

#include "assignment_instruction.h"

#include <optional>

namespace codesh::output::ir
{

class assignment_from_constant_instruction final : public assignment_instruction
{
    //FIXME: Int only
    const int constant_addition;
    const std::optional<int> constant_addition_cpi;

protected:
    void emit_rhs(std::vector<instruction_container> &collector) const override;
    [[nodiscard]] size_t rhs_size() const override;

public:
    assignment_from_constant_instruction(instruction_type type, operator_type op_type, int target_lvt_index,
            int constant_addition, std::optional<int> constant_addition_cpi);
};

}
