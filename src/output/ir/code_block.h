#pragma once

#include "instruction.h"

#include <list>
#include <memory>

namespace codesh::output::ir
{

class code_block
{
    std::list<std::unique_ptr<instruction>> instructions;

public:
    [[nodiscard]] const std::list<std::unique_ptr<instruction>> &get_instructions() const;
    void add_instruction(std::unique_ptr<instruction> instruction);
};

}
