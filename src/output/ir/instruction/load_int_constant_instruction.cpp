#include "load_int_constant_instruction.h"
#include "load_constant_pool_instruction.h"
#include "impl/instruction_container.h"
#include "impl/opcode.h"
#include "util.h"

#include <limits>
#include <stdexcept>

codesh::output::ir::load_int_constant_instruction::load_int_constant_instruction(const int constant,
        const std::optional<int> constant_cpi) :
    constant(constant),
    constant_cpi(constant_cpi)
{
}

size_t codesh::output::ir::load_int_constant_instruction::size() const
{
    if (constant_cpi.has_value())
    {
        // Constant size of load_constant_pool_instruction
        return 2;
    }


    if (-1 <= constant && constant <= 5)
    {
        return 1;
    }

    if (std::numeric_limits<int8_t>::min() <= constant && constant <= std::numeric_limits<int8_t>::max())
    {
        return 2;
    }
    if (std::numeric_limits<int16_t>::min() <= constant && constant <= std::numeric_limits<int16_t>::max())
    {
        return 3;
    }

    // If the number is greater than int16, then it must be saved in the constant pool.
    throw std::runtime_error("Attempted to load a number greater than int16 without a constant pool entry");
}

void codesh::output::ir::load_int_constant_instruction::emit(std::vector<instruction_container> &collector) const
{
    if (constant_cpi.has_value())
    {
        load_constant_pool_instruction(constant_cpi.value())
            .emit(collector);

        return;
    }


    if (-1 <= constant && constant <= 5)
    {
        collector.emplace_back(
            *opcode::I_CONST_M1 + (constant + 1),
            1
        );

        return;
    }

    if (std::numeric_limits<int8_t>::min() <= constant && constant <= std::numeric_limits<int8_t>::max())
    {
        collector.emplace_back(
            std::vector {
                *opcode::B_IPUSH,
                static_cast<unsigned char>(constant)
            },
            1
        );
    }
    else if (std::numeric_limits<int16_t>::min() <= constant && constant <= std::numeric_limits<int16_t>::max())
    {
        std::vector<unsigned char> opcodes(3);
        opcodes[0] = *opcode::S_IPUSH;
        util::put_int_bytes(opcodes.data() + 1, 2, constant);

        collector.emplace_back(std::move(opcodes), 1);
    }
    else
    {
        // If the number is greater than int16, then it must be saved in the constant pool.
        throw std::runtime_error("Attempted to load a number greater than int16 without a constant pool entry");
    }
}
