#pragma once

#include <optional>
#include <string>

namespace codesh::ast::impl
{

class i_resolvable
{
protected:
    [[nodiscard]] virtual std::optional<std::string> &get_resolved_name() = 0;

public:
    virtual ~i_resolvable();

  /**
   * @returns The unresolved type name
   */
    [[nodiscard]] virtual std::string get_name() const = 0;

    void set_resolved_name(std::string resolved_name);
    [[nodiscard]] virtual const std::optional<std::string> &get_resolved_name() const = 0;

    /**
     * @return The resolved name prefixed by the package, separated by slashes.
     */
    [[nodiscard]] std::string get_binary_name(bool resolved = true) const;
};

}