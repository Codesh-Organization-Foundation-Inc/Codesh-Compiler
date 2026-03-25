#pragma once

#include "impl/instruction.h"
#include "impl/opcode.h"
#include "defenition/primitive_type.h"

#include <vector>

namespace codesh::output::ir
{

class narrowing_cast_instruction final : public instruction
{
    std::vector<opcode> opcodes_;
    int stack_delta_;

public:
    narrowing_cast_instruction(definition::primitive_type from, definition::primitive_type to);

    [[nodiscard]] size_t size() const override;
    void emit(std::vector<instruction_container> &collector) const override;
};

}
