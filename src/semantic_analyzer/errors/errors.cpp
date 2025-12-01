#include "errors.h"

#include <stdexcept>
#include <string>

namespace codesh::errors
{


void add_error(const std::string& msg)
{
    errors_vector.push_back({ msg, 0, 0});
}

void add_error(const std::string& msg, int line, int column)
{
    errors_vector.push_back({ msg, line, column});
    //TODO: add the line and column
}

}
// void codesh::semantic_analyzer::throw_error(const std::string &msg)
// {
//     throw std::runtime_error("semantic error: " + msg);
// }
