#pragma once

#include "impl/instruction.h"

namespace codesh::output::ir
{
enum class invokation_type;


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


enum class invokation_type
{
    DYNAMIC,
    INTERFACE,
    SPECIAL, // Calls a private method_cp_index, constructor or this/super constructor
    STATIC,
    VIRTUAL
};

}
