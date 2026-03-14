#include "widening_cast_instruction.h"

#include <stdexcept>

using namespace codesh::output::ir;

struct widening_entry
{
    instruction_type from;
    instruction_type to;
    opcode op;
    int delta;
};

static constexpr widening_entry WIDENING_TABLE[] = {
    {instruction_type::INT, instruction_type::LONG, opcode::I2L, 1},
    {instruction_type::INT, instruction_type::FLOAT, opcode::I2F, 0},
    {instruction_type::INT, instruction_type::DOUBLE, opcode::I2D, 1},
    {instruction_type::LONG, instruction_type::FLOAT, opcode::L2F, -1},
    {instruction_type::LONG, instruction_type::DOUBLE, opcode::L2D, 0},
    {instruction_type::FLOAT, instruction_type::DOUBLE, opcode::F2D, 1},
};


widening_cast_instruction::widening_cast_instruction(const instruction_type from, const instruction_type to) :
    simple_instruction(lookup_opcode(from, to), lookup_delta(from, to), 1)
{
}


opcode widening_cast_instruction::lookup_opcode(const instruction_type from, const instruction_type to)
{
    for (const auto &e : WIDENING_TABLE)
    {
        if (e.from == from && e.to == to)
            return e.op;
    }

    throw std::invalid_argument("No widening opcode for this instruction_type conversion");
}

int widening_cast_instruction::lookup_delta(const instruction_type from, const instruction_type to)
{
    for (const auto &e : WIDENING_TABLE)
    {
        if (e.from == from && e.to == to)
            return e.delta;
    }

    throw std::invalid_argument("No widening delta for this instruction_type conversion");
}
