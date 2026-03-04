#pragma once

#include "impl/instruction.h"

namespace codesh::output::ir
{

class get_field_instruction final : public instruction
{
    const int constant_pool_index;

public:
    explicit get_field_instruction(int constant_pool_index);

    [[nodiscard]] size_t size() const override;

    void emit(std::vector<instruction_container> &collector) const override;
};

}
