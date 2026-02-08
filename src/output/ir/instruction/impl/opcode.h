#pragma once

namespace codesh::output::ir
{

enum class opcode : unsigned char
{
    NOP = 0x00, // No operation

    I_CONST_M1 = 0x02, // Loads -1 onto the stack

    B_IPUSH = 0x10,
    S_IPUSH, // >255

    LDC = 0x12,
    LDC2_W = 0x14,

    I_LOAD = 0x15, // Loads an integer variable from the local variable table at the specified index
    I_STORE = 0x36, // Stores an int value into variable #index

    I_RETURN = 0xAC, // Returns an int from a method

    IF_ZERO = 0x99,
    GOTO = 0xA7,

    RETURN = 0xB1,

    GET_STATIC = 0xB2,

    INVOKE_DYNAMIC = 0xBA,
    INVOKE_INTERFACE = 0xB9,
    INVOKE_SPECIAL = 0xB7,
    INVOKE_STATIC = 0xB8,
    INVOKE_VIRTUAL = 0xB6,

    I_ADD = 0x60,
    I_SUB = 0x64,
    I_MUL = 0x68,
    I_DIV = 0x6C,
    I_REM = 0x70,
    I_NEG = 0x74,
    I_SHL = 0x78,
    I_SHR = 0x7A,
    I_USHR = 0x7C,
    I_AND = 0x7E,
    I_OR = 0x80,
    I_XOR = 0x82
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

enum class operator_type
{
    ADD,
    SUB,
    MUL,
    DIV,
    REM,
    NEG,
    SHL,
    SHR,
    USHR,
    AND,
    OR,
    XOR
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

}
