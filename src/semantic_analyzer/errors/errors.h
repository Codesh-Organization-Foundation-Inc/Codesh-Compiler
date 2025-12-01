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

class error_collection
{
public:

    // Add a new error
    void add_error(const std::string& msg);

    void add_error(const std::string& msg, int line, int column);

    // Whether any errors exist
    [[nodiscard]] bool has_errors() const;

    // Print all errors
    void print_errors() const;

    // Return const reference to all errors
    [[nodiscard]] const std::vector<compiler_error>& get_errors() const { return errors; }

private:
    std::vector<compiler_error>errors_vector;

};

}

