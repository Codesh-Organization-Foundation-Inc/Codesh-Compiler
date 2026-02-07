#pragma once

#include <string>

namespace codesh::definition
{

class i_pretty_string_convertable
{
public:
    virtual ~i_pretty_string_convertable() = default;

    [[nodiscard]] virtual std::string to_pretty_string() const = 0;
};

}
