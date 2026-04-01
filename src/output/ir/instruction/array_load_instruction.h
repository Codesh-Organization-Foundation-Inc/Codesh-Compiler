#pragma once

#include "impl/instruction.h"
#include "impl/typed_instruction.h"

namespace codesh::output::ir
{

class array_load_instruction final : public instruction
{
    instruction_type type;

public:
    explicit array_load_instruction(instruction_type type);

    [[nodiscard]] size_t size() const override;

    void emit(std::vector<instruction_container> &collector) const override;
};

}
