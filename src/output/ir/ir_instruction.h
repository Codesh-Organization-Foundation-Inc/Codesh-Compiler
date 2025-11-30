#pragma once
#include <array>
#include <variant>

namespace codesh::output::jvm_target::defs
{
class cp_info;
}
namespace codesh::semantic_analyzer
{
class symbol;
}


enum class ir_instruction : unsigned char
{
    A_LOAD_0 = 0x2A, // Loads a variable from the local variable table at index 0
    RETURN = 0xB1,
    INVOKE_SPECIAL = 0xB7, // Calls for constructors, private methods and super calls
};

class three_address_code_ir
{
    using address = std::variant<codesh::semantic_analyzer::symbol &, codesh::output::jvm_target::defs::cp_info &>;

    const ir_instruction instruction_type;
    const std::array<address, 3> addresses;

public:
    three_address_code_ir(ir_instruction instruction_type, std::array<address, 3> addresses);

    codesh::semantic_analyzer::symbol &get_address_as_symbol(size_t address_index) const;
    codesh::output::jvm_target::defs::cp_info &get_address_as_cp_info(size_t address_index) const;
};
