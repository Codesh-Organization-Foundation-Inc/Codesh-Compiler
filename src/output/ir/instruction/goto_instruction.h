#pragma once

#include "impl/instruction.h"

namespace codesh::output::ir
{

class goto_instruction : public instruction
{
    int jump_offset;

public:
    explicit goto_instruction(int jump_offset);

    /**
     * Apply the JVM formula for the jump distance between the current @link jump_offset \endlink
     * and the set target
     */
    void set_target(int target);


    [[nodiscard]] size_t size() const override;
    [[nodiscard]] int get_jump_offset() const;

    void emit(std::vector<instruction_container> &collector) const override;
};

}
