#include "typed_instruction.h"
#include "instruction_container.h"

codesh::output::ir::typed_instruction::typed_instruction(const instruction_type type, const unsigned char index) :
    type(type),
    index(index)
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
            1
        );
    }
    else
    {
        collector.emplace_back(
            std::vector {
                static_cast<unsigned char>(*first_generic() + *type),
                index
            },
            1
        );
    }
}
