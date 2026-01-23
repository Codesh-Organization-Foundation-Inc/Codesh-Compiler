#pragma once

#include <list>
#include <optional>
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

    I_CONST_M1 = 0x02, // Loads -1 onto the stack

    B_IPUSH = 0x10,
    S_IPUSH, // >255

    LDC = 0x12,

    I_LOAD = 0x15, // Loads an integer variable from the local variable table at the specified index
    I_STORE = 0x36, // Stores an int value into variable #index

    IF_ZERO = 0x99,
    GOTO = 0xA7,

    RETURN = 0xB1,

    GET_STATIC = 0xB2,

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

enum class if_type
{
    IS_ZERO,
    IS_NONZERO,

    IS_NEGATIVE,
    IS_POSITIVE_OR_ZERO,
    IS_POSITIVE,
    IS_NEGATIVE_OR_ZERO,

    ARE_INTS_EQUAL,
    ARE_INTS_NOT_EQUAL,

    IS_INT_LESSER,
    IS_INT_GREATER_OR_EQUAL,
    IS_INT_GREATER,
    IS_INT_LESSER_OR_EQUAL,

    ARE_REFS_EQUAL,
    ARE_REFS_NOT_EQUAL,
};


/**
 * A shorthand for casting an opcode to a byte
 */
constexpr unsigned char operator*(const opcode op)
{
    return static_cast<unsigned char>(op);
}
constexpr unsigned char operator*(const instruction_type instr_type)
{
    return static_cast<unsigned char>(instr_type);
}
constexpr unsigned char operator*(const if_type instr_type)
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

    [[nodiscard]] virtual size_t size() const = 0;

    virtual void emit(std::list<instruction_container> &collector) const = 0;
};

class simple_instruction : public instruction
{
    const opcode _opcode;
    const int stack_delta;
    const size_t _size;

public:
    simple_instruction(opcode _opcode, int stack_delta, size_t _size);

    [[nodiscard]] opcode get_opcode() const;
    [[nodiscard]] int get_stack_delta() const;

    void emit(std::list<instruction_container> &collector) const override;
    [[nodiscard]] size_t size() const override;
};

class typed_instruction : public instruction
{
    static constexpr size_t CONSTANT_INDEXES_COUNT = 4;

    instruction_type type;
    const unsigned char index;

protected:
    [[nodiscard]] virtual opcode first_generic() const = 0;

public:
    typed_instruction(instruction_type type, unsigned char index);

    [[nodiscard]] size_t size() const override;

    void emit(std::list<instruction_container> &collector) const override;
};



class nop_instruction final : public simple_instruction
{
public:
    nop_instruction();
};


class load_instruction final : public typed_instruction
{
protected:
    [[nodiscard]] opcode first_generic() const override;

public:
    explicit load_instruction(instruction_type type, unsigned char local_var_index);
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

    [[nodiscard]] size_t size() const override;

    void emit(std::list<instruction_container> &collector) const override;
};

class load_int_constant_instruction final : public instruction
{
    const int constant;
    const std::optional<int> constant_cpi;

public:
    /**
     * @param constant
     * @param constant_cpi The constant pool index of a big integer. If passed, uses this and disregards the constant.
     */
    load_int_constant_instruction(int constant, std::optional<int> constant_cpi);

    [[nodiscard]] size_t size() const override;

    void emit(std::list<instruction_container> &collector) const override;
};

class load_constant_pool_instruction final : public instruction
{
    const int constant_pool_index;

public:
    explicit load_constant_pool_instruction(int constant_pool_index);

    [[nodiscard]] size_t size() const override;

    void emit(std::list<instruction_container> &collector) const override;
};

class store_in_local_var_instruction final : public typed_instruction
{
protected:
    [[nodiscard]] opcode first_generic() const override;

public:
    store_in_local_var_instruction(instruction_type type, int local_var_index);
};

class get_static_instruction final : public instruction
{
    const int constant_pool_index;

public:
    explicit get_static_instruction(int constant_pool_index);

    [[nodiscard]] size_t size() const override;

    void emit(std::list<instruction_container> &collector) const override;
};

class goto_instruction : public instruction
{
    int jump_offset;

public:
    explicit goto_instruction(int jump_offset);

    [[nodiscard]] size_t size() const override;

    /**
     * Apply the JVM formula for the jump distance between the current @link jump_offset \endlink
     * and the set target
     */
    void set_target(int target);

    void emit(std::list<instruction_container> &collector) const override;
};

class if_instruction final : public goto_instruction
{
    if_type type;

public:
    if_instruction(if_type type, int jump_offset);

    void emit(std::list<instruction_container> &collector) const override;
};

}
