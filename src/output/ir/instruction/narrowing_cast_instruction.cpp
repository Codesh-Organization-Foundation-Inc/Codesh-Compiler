#include "narrowing_cast_instruction.h"

#include "impl/instruction_container.h"

#include <stdexcept>
#include <unordered_map>

using namespace codesh::output::ir;
using namespace codesh::definition;

struct cast_step
{
    opcode op;
    int stack_delta;
};

struct direct_cast
{
    opcode op;
    int stack_delta;
};

static const std::unordered_map<primitive_type, cast_step> CAST_TO_INT = {
    {primitive_type::LONG, {opcode::CAST_LONG_TO_INT, -1}},
    {primitive_type::FLOAT, {opcode::CAST_FLOAT_TO_INT, 0}},
    {primitive_type::DOUBLE, {opcode::CAST_DOUBLE_TO_INT, -1}},
};

static const std::unordered_map<primitive_type, opcode> CAST_FROM_INT = {
    {primitive_type::BYTE, opcode::CAST_INT_TO_BYTE},
    {primitive_type::CHAR, opcode::CAST_INT_TO_CHAR},
    {primitive_type::SHORT, opcode::CAST_INT_TO_SHORT},
};

static const std::unordered_map<primitive_type, std::unordered_map<primitive_type, direct_cast>> DIRECT_CASTS = {
    {primitive_type::FLOAT,  {{primitive_type::LONG, {opcode::CAST_FLOAT_TO_LONG, 1}}}},
    {primitive_type::DOUBLE, {
        {primitive_type::LONG,  {opcode::CAST_DOUBLE_TO_LONG,  0}},
        {primitive_type::FLOAT, {opcode::CAST_DOUBLE_TO_FLOAT, -1}},
    }},
};

static void lookup(const primitive_type from, const primitive_type to, std::vector<opcode> &ops, int &delta)
{
    // Check direct (non-INT-routed) casts first
    if (const auto from_it = DIRECT_CASTS.find(from); from_it != DIRECT_CASTS.end())
    {
        if (const auto to_it = from_it->second.find(to); to_it != from_it->second.end())
        {
            ops.push_back(to_it->second.op);
            delta = to_it->second.stack_delta;
            return;
        }
    }

    // Wide type to int
    if (from != primitive_type::INTEGER)
    {
        const auto it = CAST_TO_INT.find(from);
        if (it == CAST_TO_INT.end())
            throw std::invalid_argument("No narrowing cast from this primitive_type");

        ops.push_back(it->second.op);
        delta = it->second.stack_delta;
    }

    // INT to sub-int
    if (to != primitive_type::INTEGER)
    {
        const auto it = CAST_FROM_INT.find(to);
        if (it == CAST_FROM_INT.end())
            throw std::invalid_argument("No narrowing cast to this primitive_type");

        ops.push_back(it->second);
    }
}

narrowing_cast_instruction::narrowing_cast_instruction(const primitive_type from, const primitive_type to) :
    stack_delta_(0)
{
    lookup(from, to, opcodes_, stack_delta_);
}

size_t narrowing_cast_instruction::size() const
{
    return opcodes_.size();
}

void narrowing_cast_instruction::emit(std::vector<instruction_container> &collector) const
{
    std::vector<unsigned char> bytes;
    bytes.reserve(opcodes_.size());
    for (const auto op : opcodes_)
    {
        bytes.push_back(*op);
    }

    collector.emplace_back(std::move(bytes), stack_delta_);
}
