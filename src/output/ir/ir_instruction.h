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


enum class ir_instruction_type : unsigned char
{
    A_LOAD_0 = 0x2A, // Loads a variable from the local variable table at index 0
    RETURN = 0xB1,
    INVOKE_SPECIAL = 0xB7, // Calls for constructors, private methods and super calls
};

class ir_instruction
{
    using address = std::variant<codesh::semantic_analyzer::symbol *, codesh::output::jvm_target::defs::cp_info *>;

    const ir_instruction_type instruction_type;
    const std::vector<address> addresses;

public:
    ir_instruction(ir_instruction_type instruction_type, std::vector<address> addresses);

    [[nodiscard]] codesh::semantic_analyzer::symbol &get_address_as_symbol(size_t address_index) const;
    [[nodiscard]] codesh::output::jvm_target::defs::cp_info &get_address_as_cp_info(size_t address_index) const;
};
