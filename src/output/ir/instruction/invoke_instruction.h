#pragma once

#include "impl/instruction.h"
#include "impl/opcode.h"

namespace codesh::output::ir
{

class invoke_instruction final : public instruction
{
    const invokation_type type;
    const int method_cp_index;
    const int parameters_count;

public:
    invoke_instruction(invokation_type type, int method_cp_index, int parameters_count);

    [[nodiscard]] size_t size() const override;

    void emit(std::vector<instruction_container> &collector) const override;
};

}
