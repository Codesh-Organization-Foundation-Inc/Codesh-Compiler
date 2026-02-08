#pragma once

#include "impl/instruction.h"

namespace codesh::output::ir
{

/**
 * An instruction that emits absolutely nothing.
 *
 * Used by the compiler to compute and determine stuff after IR emission.
 */
class marker_instruction : public instruction
{
public:
    void emit(std::vector<instruction_container> &collector) const override;
    [[nodiscard]] size_t size() const override;
};

/**
 * A marker used to indicate that the stack was additionally changed by the previous opcode
 */
class stack_size_delta_marker : public marker_instruction
{
    size_t size_delta;

public:
    explicit stack_size_delta_marker(size_t size_delta);
    void emit(std::vector<instruction_container> &collector) const override;
};

}
