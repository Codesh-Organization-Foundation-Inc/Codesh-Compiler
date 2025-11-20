#include "error.h"

#include <stdexcept>
#include <string>

namespace codesh::semantic_analyzer
{

void throw_error(const std::string& msg)
{
    throw std::runtime_error("semantic error: " + msg);
}

}
