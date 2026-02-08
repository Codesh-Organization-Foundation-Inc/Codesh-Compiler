#pragma once

#include <vector>

namespace codesh::output::ir
{

struct instruction_container;

class instruction
{
public:
    virtual ~instruction();

    [[nodiscard]] virtual size_t size() const = 0;

    virtual void emit(std::vector<instruction_container> &collector) const = 0;
};

}
