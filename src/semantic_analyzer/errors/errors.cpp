#include "errors.h"

#include <stdexcept>
#include <string>

void codesh::semantic_analyzer::throw_error(const std::string &msg)
{
    throw std::runtime_error("semantic error: " + msg);
}
