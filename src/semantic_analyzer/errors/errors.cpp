#include "errors.h"

#include <stdexcept>
#include <string>

void codesh::semantic_analyzer::collect_error(const std::string &msg)
{
    throw std::runtime_error("semantic error: " + msg);
}
