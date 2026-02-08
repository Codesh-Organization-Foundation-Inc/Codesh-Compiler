#include "marker_instruction.h"
#include "impl/instruction_container.h"

void codesh::output::ir::marker_instruction::emit(std::vector<instruction_container> &collector) const
{
}

size_t codesh::output::ir::marker_instruction::size() const
{
    return 0;
}

codesh::output::ir::stack_size_delta_marker::stack_size_delta_marker(const size_t size_delta) : size_delta(size_delta)
{
}

void codesh::output::ir::stack_size_delta_marker::emit(std::vector<instruction_container> &collector) const
{
    collector.emplace_back(std::vector<unsigned char>{}, static_cast<int>(size_delta));
}
