#include "regex.h"

#include "../token/token.h"

#include <format>
#include <sstream>
#include <vector>

using codesh::token_group;


// The order of this vector determines the order of regex priority & group number.
static const std::vector<std::pair<token_group, std::string>> TOKEN_REGEXES = {
    {token_group::LITERAL_NUMBER_FLOAT, R"(\d+(?:\.\d+)?\s+צף)"},
    {token_group::LITERAL_NUMBER_DOUBLE, R"(\d+\.\d+)"},
    {token_group::LITERAL_NUMBER_INT, R"(\d+)"},
    {token_group::LITERAL_STRING, R"(\"(?:[^\"\\]|\\.)*\")"},
    {token_group::LITERAL_CHAR, R"('(?:[^'\\]|\\.)')"},

    {token_group::IDENTIFIER, R"(\w+)"},
    {token_group::INVALID, R"(\S+)"},
};


static std::string build_single_regex(const std::pair<token_group, std::string> &token_regex);


static std::string build_lexer_regex()
{
    if (TOKEN_REGEXES.empty())
        return "";

    std::ostringstream oss;

    oss << build_single_regex(TOKEN_REGEXES[0]);

    for (size_t i = 1; i < TOKEN_REGEXES.size(); ++i)
    {
        oss << '|' << build_single_regex(TOKEN_REGEXES[i]);
    }

    return oss.str();
}

static std::string build_single_regex(const std::pair<token_group, std::string> &token_regex)
{
    std::ostringstream oss;

    oss << '(' << token_regex.second << ')';

    return oss.str();
}


const std::string codesh::LEXER_RGX_STR = build_lexer_regex();
const boost::u32regex codesh::LEXER_RGX = boost::make_u32regex(LEXER_RGX_STR);

/////

token_group codesh::token_group_from_regex_id(const int group_id)
{
    if (group_id > TOKEN_REGEXES.size())
        throw std::invalid_argument("Invalid token group id");

    return TOKEN_REGEXES[group_id - 1].first;
}
