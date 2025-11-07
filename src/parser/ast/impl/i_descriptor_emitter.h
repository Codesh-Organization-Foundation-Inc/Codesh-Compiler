#pragma once

#include <string>

namespace codesh::ast::impl
{

class i_descriptor_emitter
{
public:
    virtual ~i_descriptor_emitter() = default;

    [[nodiscard]] virtual std::string generate_descriptor() const = 0;
};

}

