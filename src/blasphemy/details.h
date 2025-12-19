#pragma once

#include <string>

namespace codesh::blasphemy::details
{

// INIT
extern const std::string SOURCE_FILE_OPEN_ERROR;
extern const std::string OUTPUT_FILE_OPEN_ERROR;
extern const std::string NO_MAIN_ARGS;

// LEXICAL
extern const std::string NO_BASAD;
extern const std::string TOKEN_DOESNT_EXIST;

// SYNTAX
extern const std::string NO_IDENTIFIER;
extern const std::string NO_IMPORT_TYPE;
extern const std::string NO_KEYWORD_LET;
extern const std::string NO_KEYWORD_NAME;
extern const std::string NO_CLOSE_MULTI_COMMENT;
extern const std::string NO_SCOPE_BEGIN;
extern const std::string NO_SCOPE_END;
extern const std::string NO_KEYWORD_SHALL_BE;
extern const std::string NO_PUNCTUATION_END_OP;
extern const std::string NO_TYPE;
extern const std::string UNEXPECTED_DECLARATION;
extern const std::string UNEXPECTED_TOKEN;

// SEMANTIC
extern const std::string UNRECOGNIZED_TYPE;

// OUTPUT
extern const std::string CONSTANT_POOL_TOO_BIG;
extern const std::string METHOD_TOO_BIG;
extern const std::string STRING_TOO_BIG;
extern const std::string NUMBER_TOO_BIG;

}
