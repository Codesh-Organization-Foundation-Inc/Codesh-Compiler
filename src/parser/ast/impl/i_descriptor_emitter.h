#pragma once

#include <string>

namespace codesh::ast::impl
{

class i_descriptor_emitter
{
public:
    virtual ~i_descriptor_emitter();

    //TODO: Just use default arguments
    /**
     * Generates a descriptor for this type.
     * @param resolved Whether to use the resolved name. False to use the flat name.
     * @return The generated descriptor
     */
    [[nodiscard]] virtual std::string generate_descriptor(bool resolved) const = 0;
    /**
     * Generates a resolved descriptor for this type.
     * @return The generated descriptor
     */
    [[nodiscard]] std::string generate_descriptor() const;
};

}
