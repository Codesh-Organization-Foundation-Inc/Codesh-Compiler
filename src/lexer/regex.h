#pragma once

#include <boost/regex/icu.hpp>
#include "../token/token.h"

namespace codesh
{

extern const std::string LEXER_RGX_STR;
extern const boost::u32regex LEXER_RGX;

token_group token_group_from_regex_id(int group_id);

}

