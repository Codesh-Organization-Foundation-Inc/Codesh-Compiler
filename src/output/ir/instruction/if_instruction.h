#pragma once

#include "goto_instruction.h"

namespace codesh::output::ir
{
enum class if_type;


class if_instruction final : public goto_instruction
{
    if_type type;

public:
    if_instruction(if_type type, int jump_offset);

    void emit(std::vector<instruction_container> &collector) const override;
};


enum class if_type
{
    IS_ZERO,
    IS_NONZERO,

    IS_NEGATIVE,
    IS_POSITIVE_OR_ZERO,
    IS_POSITIVE,
    IS_NEGATIVE_OR_ZERO,

    ARE_INTS_EQUAL,
    ARE_INTS_NOT_EQUAL,

    IS_INT_LESSER,
    IS_INT_GREATER_OR_EQUAL,
    IS_INT_GREATER,
    IS_INT_LESSER_OR_EQUAL,

    ARE_REFS_EQUAL,
    ARE_REFS_NOT_EQUAL,
};

constexpr unsigned char operator*(const if_type instr_type)
{
    return static_cast<unsigned char>(instr_type);
}

}
