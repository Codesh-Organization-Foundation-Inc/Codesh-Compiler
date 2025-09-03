#pragma once

namespace codesh::ast
{

enum class node_type
{
    COMPILATION_UNIT,
    BLOCK,
    WHILE,
    ADDITION,
    SUBTRACTION,
    MULTIPLICATION,
    DIVISION,
    MODULU,
    AND,
    EQUALS,
    GREATER_EQUALS,
    GREATER,
    LESS_EQUALS,
    LESS,
    NOT_EQUALS,
    OR,
    VARIABLE_DECLARATION,
    TYPE_DECLARATION,
};

}