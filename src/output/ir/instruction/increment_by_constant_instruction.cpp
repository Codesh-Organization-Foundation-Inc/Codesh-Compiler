#include "increment_by_constant_instruction.h"
#include "assignment_from_constant_instruction.h"

#include "util.h"

#include <cstdint>
#include <limits>

codesh::output::ir::increment_by_constant_instruction::increment_by_constant_instruction(const instruction_type type,
        const int target_lvt_index, const int constant_addition, const std::optional<int> constant_addition_cpi) :
    type(type),
    target_lvt_index(target_lvt_index),
    constant_addition(constant_addition),
    constant_addition_cpi(constant_addition_cpi)
{
}

size_t codesh::output::ir::increment_by_constant_instruction::size() const
{
    if (std::numeric_limits<uint8_t>::min() <= target_lvt_index && target_lvt_index <= std::numeric_limits<uint8_t>::max()
        && std::numeric_limits<int8_t>::min() <= constant_addition && constant_addition <= std::numeric_limits<int8_t>::max())
    {
        return 3;
    }

    if (std::numeric_limits<uint16_t>::min() <= target_lvt_index && target_lvt_index <= std::numeric_limits<uint16_t>::max()
        && std::numeric_limits<int16_t>::min() <= constant_addition && constant_addition <= std::numeric_limits<int16_t>::max())
    {
        return 6;
    }

    return assignment_from_constant_instruction(
        type,
        operator_type::ADD,
        target_lvt_index,
        constant_addition,
        constant_addition_cpi
    ).size();
}

void codesh::output::ir::increment_by_constant_instruction::emit(std::vector<instruction_container> &collector) const
{
    if (std::numeric_limits<uint8_t>::min() <= target_lvt_index && target_lvt_index <= std::numeric_limits<uint8_t>::max()
        && std::numeric_limits<int8_t>::min() <= constant_addition && constant_addition <= std::numeric_limits<int8_t>::max())
    {
        collector.emplace_back(
            std::vector {
                *opcode::I_INC,
                static_cast<unsigned char>(target_lvt_index),
                static_cast<unsigned char>(constant_addition)
            },
            0
        );
    }
    else if (std::numeric_limits<uint16_t>::min() <= target_lvt_index && target_lvt_index <= std::numeric_limits<uint16_t>::max()
        && std::numeric_limits<int16_t>::min() <= constant_addition && constant_addition <= std::numeric_limits<int16_t>::max())
    {
        std::vector<unsigned char> opcodes(6);

        opcodes[0] = *opcode::WIDE;
        opcodes[1] = *opcode::I_INC;
        util::put_int_bytes(opcodes.data() + 2, 2, target_lvt_index);
        util::put_int_bytes(opcodes.data() + 4, 2, constant_addition);

        collector.emplace_back(std::move(opcodes), 0);
    }
    else
    {
        // iterator = iterator + skip
        assignment_from_constant_instruction(
            type,
            operator_type::ADD,
            target_lvt_index,
            constant_addition,
            constant_addition_cpi
        ).emit(collector);
    }
}
