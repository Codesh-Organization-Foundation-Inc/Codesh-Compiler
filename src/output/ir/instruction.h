#pragma once

#include <list>
#include <vector>

namespace codesh::output::jvm_target
{
class constant_pool;
}
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

enum class opcode : unsigned char
{
    NOP = 0x00, // No operation

    I_CONST_M1 = 0x02,
    I_CONST_0,
    I_CONST_1,
    I_CONST_2,
    I_CONST_3,
    I_CONST_4,
    I_CONST_5,

    B_IPUSH = 0x10,
    S_IPUSH, // >255

    LDC = 0x12,

    I_LOAD = 0x15, // Loads an integer variable from the local variable table at the specified index
    L_LOAD, // Loads a long variable from the local variable table at the specified index
    F_LOAD, // Loads a float variable from the local variable table at the specified index
    D_LOAD, // Loads a double variable from the local variable table at the specified index
    A_LOAD, // Loads a reference variable from the local variable table at the specified index

    I_LOAD_0 = 0x1A, // Loads an integer variable from the local variable table at index 0
    I_LOAD_1, // Loads an integer variable from the local variable table at index 1
    I_LOAD_2, // Loads an integer variable from the local variable table at index 2
    I_LOAD_3, // Loads an integer variable from the local variable table at index 3

    L_LOAD_0, // Loads a long variable from the local variable table at index 0
    L_LOAD_1, // Loads a long variable from the local variable table at index 1
    L_LOAD_2, // Loads a long variable from the local variable table at index 2
    L_LOAD_3, // Loads a long variable from the local variable table at index 3

    F_LOAD_0, // Loads a float variable from the local variable table at index 0
    F_LOAD_1, // Loads a float variable from the local variable table at index 1
    F_LOAD_2, // Loads a float variable from the local variable table at index 2
    F_LOAD_3, // Loads a float variable from the local variable table at index 3

    D_LOAD_0, // Loads a double variable from the local variable table at index 0
    D_LOAD_1, // Loads a double variable from the local variable table at index 1
    D_LOAD_2, // Loads a double variable from the local variable table at index 2
    D_LOAD_3, // Loads a double variable from the local variable table at index 3

    A_LOAD_0, // Loads a variable from the local variable table at index 0
    A_LOAD_1, // Loads a variable from the local variable table at index 1
    A_LOAD_2, // Loads a variable from the local variable table at index 2
    A_LOAD_3, // Loads a variable from the local variable table at index 3

    RETURN = 0xB1,

    INVOKE_DYNAMIC = 0xBA,
    INVOKE_INTERFACE = 0xB9,
    INVOKE_SPECIAL = 0xB7,
    INVOKE_STATIC = 0xB8,
    INVOKE_VIRTUAL = 0xB6
};

enum class instruction_type
{
    INT,
    LONG,
    FLOAT,
    DOUBLE,
    REFERENCE
};

enum class invokation_type
{
    DYNAMIC,
    INTERFACE,
    SPECIAL, // Calls a private method_cp_index, constructor or this/super constructor
    STATIC,
    VIRTUAL
};


constexpr unsigned char operator*(const opcode op)
{
    return static_cast<unsigned char>(op);
}
constexpr unsigned char operator*(const instruction_type instr_type)
{
    return static_cast<unsigned char>(instr_type);
}


struct instruction_container
{
    std::vector<unsigned char> opcodes;

    /**
     * Signals the change in stack size this instruction commits.
     */
    int size_delta;

    instruction_container(std::vector<unsigned char> opcodes, int size_delta);
    instruction_container(unsigned char opcode, int size_delta);
};

class instruction
{
public:
    virtual ~instruction();

    virtual void emit(std::list<instruction_container> &collector) const = 0;
};

class simple_instruction : public instruction
{
    const opcode _opcode;
    const int stack_delta;

public:
    simple_instruction(opcode _opcode, int stack_delta);

    [[nodiscard]] opcode get_opcode() const;
    [[nodiscard]] int get_stack_delta() const;

    void emit(std::list<instruction_container> &collector) const override;
};

class typed_instruction : public instruction
{
    instruction_type type;

public:
    explicit typed_instruction(instruction_type type);

    [[nodiscard]] instruction_type get_instruction_type() const;
};



class nop_instruction final : public simple_instruction
{
public:
    nop_instruction();
};


class load_instruction final : public typed_instruction
{
    static constexpr size_t CONSTANT_INDEXES_COUNT = 4;

    //TODO: Change to a Local Variable Table index pointer
    const unsigned char lvt_index;

public:
    explicit load_instruction(instruction_type type, unsigned char lvt_index);

    [[nodiscard]] unsigned char get_lvt_index() const;

    void emit(std::list<instruction_container> &collector) const override;
};


//TODO: Make typed version
class return_instruction final : public simple_instruction
{
public:
    return_instruction();
};


class invoke_instruction final : public instruction
{
    const invokation_type type;
    const int method_cp_index;
    const int parameters_count;

public:
    invoke_instruction(invokation_type type, int method_cp_index, int parameters_count);

    void emit(std::list<instruction_container> &collector) const override;

    [[nodiscard]] int get_method_cp_index() const;
};

class load_int_constant_instruction final : public instruction
{
    const int constant;
    const jvm_target::constant_pool &fallback_constant_pool;

public:
    load_int_constant_instruction(int constant, const jvm_target::constant_pool &fallback_constant_pool);

    void emit(std::list<instruction_container> &collector) const override;
};

class load_constant_pool_instruction final : public instruction
{
    const int constant_pool_index;

public:
    explicit load_constant_pool_instruction(int constant_pool_index);

    void emit(std::list<instruction_container> &collector) const override;
};

}
