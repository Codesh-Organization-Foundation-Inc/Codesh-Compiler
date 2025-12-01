#include "errors.h"
#include <iostream>

namespace codesh::semantic_analyzer
{

void error_collector::add_error(const std::string &msg)
{
    errors_vector.push_back({ msg, 0, 0 });
}

void error_collector::add_error(const std::string &msg, const int line, const int column)
{
    errors_vector.push_back({ msg, line, column });
}

bool error_collector::has_errors() const
{
    return !errors_vector.empty();
}

void error_collector::print_errors() const
{
    for (const auto &[message, line, column] : errors_vector)
    {
        std::cerr
            << "Error at line " << line
            << ", column " << column
            << ": " << message;
        // TODO: change it
    }
}

}