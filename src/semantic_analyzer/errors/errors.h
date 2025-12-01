#pragma once

#include <string>
#include <vector>

namespace codesh::semantic_analyzer
{

struct compiler_error
{
    std::string message;
    int line;
    int column;
};

class error_collector
{
public:

    // Add a new error
    void add_error(const std::string &msg);

    void add_error(const std::string &msg, int line, int column);

    // Whether any errors exist
    [[nodiscard]] bool has_errors() const;

    // Print all errors
    void print_errors() const;

private:
    std::vector<compiler_error> errors_vector;

};

}

