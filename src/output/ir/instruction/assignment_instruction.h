#pragma once

#include "impl/instruction.h"
#include "impl/opcode.h"
#include "impl/typed_instruction.h"

namespace codesh::output::ir
{

class assignment_instruction : public instruction
{
    const int target_lvt_index;
    operator_type op_type;

protected:
    instruction_type type;

    assignment_instruction(instruction_type type, operator_type op_type, int target_lvt_index);

    virtual void emit_rhs(std::vector<instruction_container> &collector) const = 0;
    [[nodiscard]] virtual size_t rhs_size() const = 0;

public:
    [[nodiscard]] size_t size() const override;

    void emit(std::vector<instruction_container> &collector) const override;
};

}
