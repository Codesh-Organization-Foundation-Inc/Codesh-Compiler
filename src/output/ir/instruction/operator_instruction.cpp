#include "operator_instruction.h"

#include <stdexcept>

codesh::output::ir::operator_instruction::operator_instruction(const instruction_type type, const operator_type op) :
    typed_instruction(type, 0),
    op(op)
{
}

codesh::output::ir::opcode codesh::output::ir::operator_instruction::first_generic() const
{
    switch (op)
    {
        case operator_type::ADD: return opcode::I_ADD;
        case operator_type::SUB: return opcode::I_SUB;
        case operator_type::MUL: return opcode::I_MUL;
        case operator_type::DIV: return opcode::I_DIV;
        case operator_type::REM: return opcode::I_REM;
        case operator_type::NEG: return opcode::I_NEG;
        case operator_type::SHL: return opcode::I_SHL;
        case operator_type::SHR: return opcode::I_SHR;
        case operator_type::USHR: return opcode::I_USHR;
        case operator_type::AND: return opcode::I_AND;
        case operator_type::OR: return opcode::I_OR;
        case operator_type::XOR: return opcode::I_XOR;

        default: throw std::runtime_error("Unknown operator type");
    }
}

size_t codesh::output::ir::operator_instruction::size() const
{
    return 1;
}

void codesh::output::ir::operator_instruction::emit(std::vector<instruction_container> &collector) const
{
    collector.emplace_back(
        static_cast<unsigned char>(*first_generic() + *type),
        op == operator_type::NEG ? 0 : -1
    );
}
