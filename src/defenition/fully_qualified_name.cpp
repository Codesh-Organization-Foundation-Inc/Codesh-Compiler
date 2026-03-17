#include "fully_qualified_name.h"

#include "fmt/xchar.h"
#include "lexer/trie/keywords.h"
#include "semantic_analyzer/builtins.h"

#include <sstream>

codesh::definition::fully_qualified_name::fully_qualified_name(const lexer::code_position code_position) :
    _is_wildcard(false),
    code_position(code_position)
{
}

codesh::definition::fully_qualified_name::fully_qualified_name(const lexer::code_position code_position,
        std::string part) :
    fully_qualified_name(code_position)
{
    add(std::move(part));
}

codesh::definition::fully_qualified_name::fully_qualified_name(const lexer::code_position code_position,
        const std::vector<std::string>::const_iterator name_start,
        const std::vector<std::string>::const_iterator name_end) :
    parts(name_start, name_end),
    _is_wildcard(false),
    code_position(code_position)
{
}

codesh::definition::fully_qualified_name codesh::definition::fully_qualified_name::parse(
        const std::string &fqn_str, const lexer::code_position code_position)
{
    fully_qualified_name result(code_position);
    std::istringstream fqn_stream(fqn_str);

    // Split by '/'
    std::string item;
    while (std::getline(fqn_stream, item, '/'))
    {
        if (!item.empty())
        {
            result.add(item);
        }
    }

    return result;
}

bool codesh::definition::fully_qualified_name::operator==(const fully_qualified_name &other) const
{
    return parts == other.parts && _is_wildcard == other._is_wildcard;
}

codesh::definition::fully_qualified_name codesh::definition::fully_qualified_name::with(std::string part)
    const
{
    fully_qualified_name result = *this;
    result.add(std::move(part));

    return result;
}

codesh::definition::fully_qualified_name codesh::definition::fully_qualified_name::omit_last() const
{
    fully_qualified_name result = *this;
    result.parts.pop_back();

    return result;
}

void codesh::definition::fully_qualified_name::add(std::string part)
{
    parts.emplace_back(std::move(part));
}

const std::vector<std::string> &codesh::definition::fully_qualified_name::get_parts() const
{
    return parts;
}

void codesh::definition::fully_qualified_name::set_code_position(const lexer::code_position pos)
{
    code_position = pos;
}

codesh::lexer::code_position codesh::definition::fully_qualified_name::get_code_position() const
{
    return code_position;
}

void codesh::definition::fully_qualified_name::set_is_wildcard(const bool wildcard)
{
    _is_wildcard = wildcard;
}

bool codesh::definition::fully_qualified_name::is_wildcard() const
{
    return _is_wildcard;
}

bool codesh::definition::fully_qualified_name::is_single_part() const
{
    return parts.size() == 1;
}

std::string codesh::definition::fully_qualified_name::get_last_part() const
{
    return parts.back();
}

std::string codesh::definition::fully_qualified_name::join(const std::string &sep) const
{
    return fmt::format("{}",
        fmt::join(parts, sep)
    );
}

std::string codesh::definition::fully_qualified_name::holy_join() const
{
    if (const auto result = parse_alias())
    {
        return result.value();
    }

    fully_qualified_name pretty_fqn(code_position);
    for (const auto &part : get_parts())
    {
        if (part == "this")
        {
            pretty_fqn.add(lexer::trie::TOKEN_TO_NAME_MAP.at(token_group::KEYWORD_THIS));
        }
        else
        {
            pretty_fqn.add(part);
        }
    }

    return pretty_fqn.join(" ל־");
}

std::optional<std::string> codesh::definition::fully_qualified_name::parse_alias() const
{
    const auto joined = join();

    if (joined == "java/lang/String")
        return semantic_analyzer::builtins::ALIAS_STRING;
    if (joined == "java/lang/Object")
        return semantic_analyzer::builtins::ALIAS_OBJECT;

    return std::nullopt;
}
