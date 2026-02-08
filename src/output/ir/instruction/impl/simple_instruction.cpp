#include "simple_instruction.h"
#include "instruction_container.h"

codesh::output::ir::simple_instruction::simple_instruction(opcode _opcode, int stack_delta, const size_t _size) :
    _opcode(_opcode),
    stack_delta(stack_delta),
    _size(_size)
{
}

codesh::output::ir::opcode codesh::output::ir::simple_instruction::get_opcode() const
{
    return _opcode;
}

int codesh::output::ir::simple_instruction::get_stack_delta() const
{
    return stack_delta;
}

void codesh::output::ir::simple_instruction::emit(std::vector<instruction_container> &collector) const
{
    collector.emplace_back(*_opcode, stack_delta);
}

size_t codesh::output::ir::simple_instruction::size() const
{
    return _size;
}
