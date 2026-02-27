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

    NEW = 0xBB,
    POP = 0x57,
    DUP = 0x59,

    GET_STATIC = 0xB2,
    PUT_STATIC = 0xB3,
    GET_FIELD  = 0xB4,

    INVOKE_DYNAMIC = 0xBA,
    INVOKE_INTERFACE = 0xB9,
    INVOKE_SPECIAL = 0xB7,
    INVOKE_STATIC = 0xB8,
    INVOKE_VIRTUAL = 0xB6,

    WIDE = 0xC4,

    I_INC = 0x84,

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

enum class operator_type
{
    ASSIGN,
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

}
