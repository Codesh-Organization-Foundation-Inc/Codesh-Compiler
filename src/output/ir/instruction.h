#pragma once

#include <variant>
#include <vector>

namespace codesh::output::jvm_target::defs
{
class cp_info;
}
namespace codesh::semantic_analyzer
{
class symbol;
}


namespace codesh::output::ir
{

using operand = std::variant<
    semantic_analyzer::symbol *,
    jvm_target::defs::cp_info *
>;


enum class opcode : unsigned char
{
    NOP = 0x00, // No operation
    A_LOAD_0 = 0x2A, // Loads a variable from the local variable table at index 0
    RETURN = 0xB1,
    INVOKE_SPECIAL = 0xB7, // Calls for constructors, private methods and super calls
};

class instruction
{
    const opcode instruction_type;
    const std::vector<operand> operands;

public:
    instruction(opcode instruction_type, std::vector<operand> operands);

    [[nodiscard]] semantic_analyzer::symbol &get_address_as_symbol(size_t address_index) const;
    [[nodiscard]] jvm_target::defs::cp_info &get_address_as_cp_info(size_t address_index) const;
};

}
