#include "fully_qualified_name.h"

#include "fmt/xchar.h"

#include <sstream>

codesh::definition::fully_qualified_name::fully_qualified_name() :
    _is_wildcard(false)
{
}

codesh::definition::fully_qualified_name::fully_qualified_name(const char *binary_fqn) :
    fully_qualified_name()
{
    std::istringstream fqn_stream(binary_fqn);

    // Split by '/'
    std::string item;
    while (std::getline(fqn_stream, item, '/'))
    {
        if (!item.empty())
        {
            add(item);
        }
    }
}

codesh::definition::fully_qualified_name::fully_qualified_name(std::string part) :
    fully_qualified_name()
{
    add(std::move(part));
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

void codesh::definition::fully_qualified_name::set_is_wildcard(bool wildcard)
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
    return join(" ל־");
}
