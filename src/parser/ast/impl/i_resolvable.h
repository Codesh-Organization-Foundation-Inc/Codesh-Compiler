#pragma once

#include "../../../defenition/fully_qualified_class_name.h"

#include <optional>

namespace codesh::ast::impl
{

class i_resolvable
{
protected:
    [[nodiscard]] virtual std::optional<definition::fully_qualified_class_name> &_get_resolved_name() = 0;
    [[nodiscard]] virtual const std::optional<definition::fully_qualified_class_name> &_get_resolved_name() const = 0;

public:
    virtual ~i_resolvable();

    /**
     * @returns The unresolved type name
     */
    [[nodiscard]] virtual const definition::fully_qualified_class_name &get_name() const = 0;
    /**
     * @param resolved When true, behaves like {@link get_resolved_name}. When false, behaves like {@link get_name}
     */
    [[nodiscard]] const definition::fully_qualified_class_name &get_name(bool resolved) const;

    //FIXME: This should be FQCN
    void set_resolved_name(definition::fully_qualified_class_name resolved_name);
    [[nodiscard]] const definition::fully_qualified_class_name &get_resolved_name() const;
};

}