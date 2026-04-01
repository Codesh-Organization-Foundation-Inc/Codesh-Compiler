#pragma once

#include "defenition/primitive_type.h"
#include "impl/instruction.h"

namespace codesh::output::ir
{

class new_array_instruction final : public instruction
{
    const definition::primitive_type type;

public:
    explicit new_array_instruction(definition::primitive_type type);

    [[nodiscard]] size_t size() const override;

    void emit(std::vector<instruction_container> &collector) const override;
};

}
