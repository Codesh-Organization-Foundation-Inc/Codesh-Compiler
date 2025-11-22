#pragma once

#include <string>

namespace codesh::ast::impl
{

class i_resolvable
{
public:
    virtual ~i_resolvable() = default;

    virtual void set_resolved_name(std::string resolved_name) = 0;

    /**
     * @return The resolved name prefixed by the package, separated by slashes.
     */
    [[nodiscard]] virtual std::string get_binary_name() const = 0;
};

}