#pragma once

#include "instruction_container.h"

#include <vector>

namespace codesh::output::ir
{

class instruction
{
public:
    virtual ~instruction();

    [[nodiscard]] virtual size_t size() const = 0;

    virtual void emit(std::vector<instruction_container> &collector) const = 0;
};

}
