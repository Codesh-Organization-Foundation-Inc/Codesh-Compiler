#include "fully_qualified_class_name.h"

#include <sstream>

codesh::definition::fully_qualified_class_name::fully_qualified_class_name() :
    _is_wildcard(false)
{
}

void codesh::definition::fully_qualified_class_name::add(std::string part)
{
    parts.emplace_back(std::move(part));
}

const std::list<std::string> &codesh::definition::fully_qualified_class_name::get_parts() const
{
    return parts;
}

void codesh::definition::fully_qualified_class_name::set_is_wildcard(bool wildcard)
{
    _is_wildcard = wildcard;
}

bool codesh::definition::fully_qualified_class_name::is_wildcard() const
{
    return _is_wildcard;
}

std::string codesh::definition::fully_qualified_class_name::join(const char sep) const
{
    std::ostringstream builder;

    for (const auto &part : parts)
    {
        builder << sep << part;
    }

    return builder.str();
}
