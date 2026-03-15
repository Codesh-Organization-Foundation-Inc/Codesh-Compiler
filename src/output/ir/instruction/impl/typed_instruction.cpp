#include "typed_instruction.h"

codesh::output::ir::typed_instruction::typed_instruction(const instruction_type type, const unsigned char index) :
    index(index),
    stack_delta(get_stack_delta(type)),
    type(type)
{
}

size_t codesh::output::ir::typed_instruction::size() const
{
    return index <= 3 ? 1 : 2;
}

void codesh::output::ir::typed_instruction::emit(std::vector<instruction_container> &collector) const
{
    if (index <= 3)
    {
        const unsigned char first_non_generic = *first_generic() + CONSTANT_INDEXES_COUNT + 1;

        collector.emplace_back(
            first_non_generic
                + *type * CONSTANT_INDEXES_COUNT
                + index,
            stack_delta
        );
    }
    else
    {
        collector.emplace_back(
            std::vector {
                static_cast<unsigned char>(*first_generic() + *type),
                index
            },
            stack_delta
        );
    }
}

size_t codesh::output::ir::typed_instruction::get_stack_delta(const instruction_type type)
{
    switch (type)
    {
    case instruction_type::DOUBLE:
    case instruction_type::LONG:
        return 2;

    default:
        return 1;
    }
}
