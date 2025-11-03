#pragma once

#include <boost/regex/icu.hpp>
#include "../token/token.h"

namespace codesh::lexer
{

extern const std::string LEXER_RGX_STR;
extern const boost::u32regex LEXER_RGX;

/**
 * The first token group in a sequence of token groups
 * that match a REGEX group.
 */
extern const token_group TOKEN_GROUP_RGX_START;
/**
 * The total amount of consecutive token groups that the REGEX can match
 */
extern const size_t TOKEN_GROUP_RGX_COUNT;

token_group token_group_from_regex_id(int group_id);

}

