#pragma once

#include <string>

namespace codesh
{
enum class token_group : int;
}


namespace codesh
{

extern const std::string LEXER_RGX;

token_group token_group_from_regex_id(int group_id);

}

