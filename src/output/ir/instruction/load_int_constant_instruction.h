#pragma once

#include "impl/instruction.h"

#include <optional>

namespace codesh::output::ir
{

class load_int_constant_instruction final : public instruction
{
    const int constant;
    const std::optional<int> constant_cpi;

public:
    /**
     * @param constant
     * @param constant_cpi The constant pool index of a big integer. If passed, uses this and disregards the constant.
     */
    load_int_constant_instruction(int constant, std::optional<int> constant_cpi);

    [[nodiscard]] size_t size() const override;

    void emit(std::vector<instruction_container> &collector) const override;
};

}
