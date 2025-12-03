#pragma once

#include <optional>
#include <string>
#include <vector>

namespace codesh
{

// TODO: move the file out and make it that there will be error type (semantic, parser...)
struct code_position
{
    size_t line;
    size_t column;
};

struct blasphemy
{
    std::string message;
    std::optional<code_position> code_pos;
};


class blasphemy_collector
{
public:

    /**
     * Add a new error
     */
    void add_blasphemy(std::string msg);
    /**
     * Add a new error, specifying its source location
     */
    void add_blasphemy(std::string msg, code_position code_pos);

    /**
     * Whether any errors exist
     */
    [[nodiscard]] bool has_errors() const;

    /**
     * Print all errors
     */
    void print_all_errors() const;

private:
    std::vector<blasphemy> blasphemies;

};

}

