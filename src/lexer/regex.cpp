#include "regex.h"

#include "fmt/xchar.h"

#include <ranges>
#include <vector>

using codesh::token_group;


// The order of this vector determines the order of regex priority & group number.
static const std::vector<std::pair<token_group, std::string>> TOKEN_REGEXES = {
    {token_group::LITERAL_NUMBER_FLOAT, R"(\d+(?:\.\d+)?\s+צף)"},
    {token_group::LITERAL_NUMBER_DOUBLE, R"(\d+\.\d+)"},
    {token_group::LITERAL_NUMBER_INT, R"(\d+)"},
    {token_group::LITERAL_STRING, R"(ויקרא(?: | [^\n]+? )(?<! ליטרלי )לאמר)"},
    {token_group::LITERAL_CHAR, R"('(?:[^'\\]|\\.)')"},

    {token_group::IDENTIFIER, R"([^\s.;[\]/<>:]+)"},
    {token_group::INVALID, R"(\S+)"},
};

const token_group codesh::lexer::TOKEN_GROUP_RGX_START = TOKEN_REGEXES.front().first;
const size_t codesh::lexer::TOKEN_GROUP_RGX_COUNT = TOKEN_REGEXES.size();


static std::string build_single_regex(const std::pair<token_group, std::string> &token_regex);


static std::string build_lexer_regex()
{
    if (TOKEN_REGEXES.empty())
        return "";

    auto regexes = TOKEN_REGEXES | std::views::transform(build_single_regex);

    return fmt::format(
        "{}",
        fmt::join(regexes, "|")
    );
}

static std::string build_single_regex(const std::pair<token_group, std::string> &token_regex)
{
    return fmt::format("({})", token_regex.second);
}


const std::string codesh::lexer::LEXER_RGX_STR = build_lexer_regex();
const boost::u32regex codesh::lexer::LEXER_RGX = boost::make_u32regex(LEXER_RGX_STR);

/////

token_group codesh::lexer::token_group_from_regex_id(const int group_id)
{
    if (group_id > TOKEN_GROUP_RGX_COUNT)
        throw std::invalid_argument("Invalid token group id");

    return static_cast<token_group>(static_cast<size_t>(TOKEN_GROUP_RGX_START) + group_id - 1);
}
