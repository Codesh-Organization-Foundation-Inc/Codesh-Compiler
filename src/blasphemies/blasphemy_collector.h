#pragma once

#include <optional>
#include <string>
#include <vector>

namespace codesh::error
{

enum class blasphemy_type
{
    LEXICAL,
    SYNTAX,
    SEMANTIC,

    UNKNOWN
};

struct code_position
{
    size_t line;
    size_t column;
};

struct blasphemy
{
    std::string message;
    blasphemy_type type;
    std::optional<code_position> code_pos;
};


class blasphemy_collector
{
public:

    /**
     * Adds a new error with only a name
     */
    void add_blasphemy(std::string msg);
    /**
     * Adds a new typed error
     */
    void add_blasphemy(std::string msg, blasphemy_type type);
    /**
     * Adds a new typed error with a known code position
     */
    void add_blasphemy(std::string msg, blasphemy_type type, code_position code_pos);

    /**
     * Whether any error exists
     */
    [[nodiscard]] bool has_errors() const;

    /**
     * Prints all errors
     */
    void print_all_errors() const;

private:
    std::vector<blasphemy> blasphemies;

    static std::string type_to_string(blasphemy_type type);

    [[nodiscard]] static std::string get_blasphemy_message(blasphemy_type type, size_t line,
                                                    const std::string &file_name);

    [[nodiscard]] static std::string get_random_message();

};


blasphemy_collector &get_blasphemy_collector();

}

