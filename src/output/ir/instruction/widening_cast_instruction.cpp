#include "widening_cast_instruction.h"

#include <stdexcept>
#include <unordered_map>

using namespace codesh::output::ir;

struct widening_entry
{
    opcode op;
    int stack_delta;
};

static const std::unordered_map<instruction_type, std::unordered_map<instruction_type, widening_entry>> WIDENING_MAP = {
    {
        instruction_type::INT, {
            {instruction_type::LONG, {opcode::I2L, 1}},
            {instruction_type::FLOAT, {opcode::I2F, 0}},
            {instruction_type::DOUBLE, {opcode::I2D, 1}}
        }
    },
    {
        instruction_type::LONG, {
            {instruction_type::FLOAT, {opcode::L2F, -1}},
            {instruction_type::DOUBLE, {opcode::L2D, 1}}
        }
    },
    {
        instruction_type::FLOAT, {
            {instruction_type::DOUBLE, {opcode::F2D, 1}}
        }
    },
};


widening_cast_instruction::widening_cast_instruction(const instruction_type from, const instruction_type to) :
    simple_instruction(lookup_opcode(from, to), lookup_delta(from, to), 1)
{
}


static const widening_entry &lookup_entry(const instruction_type from, const instruction_type to)
{
    const auto from_entry = WIDENING_MAP.find(from);
    if (from_entry != WIDENING_MAP.end())
    {
        const auto to_entry = from_entry->second.find(to);
        if (to_entry != from_entry->second.end())
            return to_entry->second;
    }

    throw std::invalid_argument("No widening entry for this instruction_type conversion");
}

opcode widening_cast_instruction::lookup_opcode(const instruction_type from, const instruction_type to)
{
    return lookup_entry(from, to).op;
}

int widening_cast_instruction::lookup_delta(const instruction_type from, const instruction_type to)
{
    return lookup_entry(from, to).stack_delta;
}
