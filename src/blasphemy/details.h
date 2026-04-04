#pragma once

#include "fmt/base.h"

#include <string>

namespace codesh::blasphemy::details
{

// INIT
extern const std::string SRC_NOT_PROVIDED;
extern const std::string DEST_NOT_PROVIDED;
extern const fmt::runtime_format_string<> SOURCE_FILE_OPEN_ERROR;
extern const fmt::runtime_format_string<> OUTPUT_FILE_OPEN_ERROR;
extern const fmt::runtime_format_string<> DEST_PATH_NOT_DIRECTORY;
extern const fmt::runtime_format_string<> INVALID_ARG;
extern const fmt::runtime_format_string<> PATH_DOESNT_EXIST;
extern const std::string NO_MAIN_ARGS;
extern const std::string NO_ARG;
extern const std::string UNKNOWN_FLAG;
extern const fmt::runtime_format_string<> JAR_COMMAND_FAILED;

// LEXICAL
extern const std::string NO_BASAD;
extern const std::string TOKEN_DOESNT_EXIST;
extern const std::string BAD_BASAD;

// SYNTAX
extern const std::string NO_IDENTIFIER;
extern const std::string NO_IMPORT_TYPE;
extern const std::string NO_KEYWORD_LET;
extern const std::string NO_KEYWORD_NAME;
extern const std::string NO_KEYWORD_NAME_OR_CONSTRUCTOR;
extern const std::string NO_CLOSE_MULTI_COMMENT;
extern const std::string NO_CLOSE_PARENTHESIS;
extern const std::string NO_SCOPE_BEGIN;
extern const std::string NO_SCOPE_END;
extern const std::string NO_KEYWORD_SHALL_BE;
extern const std::string NO_KEYWORD_PUNC_EQUAL;
extern const std::string NO_KEYWORD_BY;
extern const std::string NO_KEYWORD_THE;
extern const std::string NO_KEYWORD_TO;
extern const std::string NO_KEYWORD_FROM;
extern const std::string NO_KEYWORD_SKIP;
extern const std::string NO_KEYWORD_CUBITS;
extern const std::string NO_KEYWORD_HIS_LENGTH;
extern const std::string NO_KEYWORD_AGAINST;
extern const std::string NO_PUNCTUATION_END_OP;
extern const std::string NO_TYPE;
extern const std::string UNEXPECTED_DECLARATION;
extern const std::string UNEXPECTED_TOKEN;
extern const std::string EXPECTED_VARIABLE;
extern const std::string INVALID_TYPE_FOR_NEW;
extern const std::string NO_SHIFTING_DIRECTION;

// SEMANTIC
extern const std::string UNRECOGNIZED_TYPE;
extern const std::string CONDITION_NOT_BOOLEAN;
extern const std::string ITERATOR_NOT_COLLECTION;
extern const fmt::runtime_format_string<> NOT_AN_OBJECT;
extern const fmt::runtime_format_string<> UNARY_TYPE_MISMATCH;
extern const fmt::runtime_format_string<> BINARY_TYPE_MISMATCH;
extern const fmt::runtime_format_string<> VARIABLE_REFERENCED_BEFORE_CREATION;
extern const fmt::runtime_format_string<> NOT_A_VARIABLE;
extern const fmt::runtime_format_string<> SYMBOL_NOT_FOUND;
extern const fmt::runtime_format_string<> TYPE_DOES_NOT_EXIST;
extern const fmt::runtime_format_string<> METHOD_NOT_FOUND;
extern const fmt::runtime_format_string<> NOT_A_METHOD;
extern const fmt::runtime_format_string<> UNEXPECTED_END_OF_NUMBER;
extern const fmt::runtime_format_string<> INVALID_NUMBER_FORMAT_ASCENDING;
extern const fmt::runtime_format_string<> INVALID_MULTIPLICATION_FACTOR;
extern const fmt::runtime_format_string<> INVALID_NUMBER_FORMAT_DESCENDING;
extern const fmt::runtime_format_string<> UNEXPECTED_ADDITION_OPERATOR;
extern const std::string UNEXPECTED_PERIOD_OPERATOR;
extern const std::string MULTIPLE_PERIOD_OPERATORS;
extern const fmt::runtime_format_string<> ARGUMENT_TYPE_MISMATCH;
extern const fmt::runtime_format_string<> RETURN_TYPE_MISMATCH;
extern const fmt::runtime_format_string<> DUPLICATE_TYPE_DECLARATION;
extern const fmt::runtime_format_string<> DUPLICATE_METHOD_DECLARATION;
extern const fmt::runtime_format_string<> NON_STATIC_CALL_FROM_STATIC_CONTEXT;
extern const fmt::runtime_format_string<> NOT_AN_ARRAY;
extern const std::string ARRAY_INDEX_NOT_INTEGER;
extern const fmt::runtime_format_string<> INVALID_CAST;
extern const fmt::runtime_format_string<> THROWS_NOT_EXCEPTION;
extern const fmt::runtime_format_string<> UNDECLARED_SIN;
extern const fmt::runtime_format_string<> DUPLICATE_INTERFACE;
extern const fmt::runtime_format_string<> UNIMPLEMENTED_METHOD;
extern const std::string DUPLICATE_MAIN_CLASS;
extern const fmt::runtime_format_string<> MAIN_CLASS_NOT_FOUND;

// EXTERNAL
extern const fmt::runtime_format_string<> CLASSPATH_NOT_SUPPORTED;
extern const fmt::runtime_format_string<> CLASSPATH_OPEN_ERROR;

// OUTPUT
extern const std::string CONSTANT_POOL_TOO_BIG;
extern const std::string METHOD_TOO_BIG;
extern const std::string STRING_TOO_BIG;
extern const std::string NUMBER_TOO_BIG;
extern const fmt::runtime_format_string<> TOO_MANY_LOCAL_VARIABLES;

// WARNINGS
extern const fmt::runtime_format_string<> NON_BIBLICAL_NUMBER;
extern const fmt::runtime_format_string<> INTEROP_REPLACEMENT_EXISTS;
extern const fmt::runtime_format_string<> ILLEGAL_IDENTIFIER_CONTENT_FOR_NON_KOSHER_LANGUAGES;

}
