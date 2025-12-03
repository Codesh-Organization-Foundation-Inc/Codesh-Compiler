#pragma once

#include <optional>
#include <string>
#include <vector>

namespace codesh
{

enum class blasphemy_type
{
    Lexical,
    Syntax,
    Semantic
};

struct code_position
{
    size_t line;
    size_t column;
};

struct blasphemy
{
    std::string message;
    std::optional<blasphemy_type> type;
    std::optional<code_position> code_pos;
};


class blasphemy_collector
{
public:

    /**
     * Add a new error with only name
     */
    void add_blasphemy(std::string msg);
    /**
     * Add a new error name and type
     */
    void add_blasphemy(blasphemy_type type, std::string msg);
    /**
     * Add a new error, specifying its source location and type
     */
    void add_blasphemy(blasphemy_type type,std::string msg, code_position code_pos);

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

    static std::string type_to_string(blasphemy_type type);

};

}

