#pragma once

#include <vector>

namespace codesh::output::ir
{

struct instruction_container
{
    std::vector<unsigned char> opcodes;

    /**
     * Signals the change in stack size this instruction commits.
     */
    int size_delta;

    instruction_container(std::vector<unsigned char> opcodes, int size_delta);
    instruction_container(unsigned char opcode, int size_delta);
};

}
