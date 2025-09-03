#pragma once

namespace codesh::ast
{

enum class node_type
{
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
    OR
};

}