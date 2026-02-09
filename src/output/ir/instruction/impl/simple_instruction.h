#pragma once

#include "instruction.h"
#include "opcode.h"

namespace codesh::output::ir
{

class simple_instruction : public instruction
{
    const opcode _opcode;
    const int stack_delta;
    const size_t _size;

public:
    simple_instruction(opcode _opcode, int stack_delta, size_t _size);

    [[nodiscard]] opcode get_opcode() const;
    [[nodiscard]] int get_stack_delta() const;

    void emit(std::vector<instruction_container> &collector) const override;
    [[nodiscard]] size_t size() const override;
};

}
