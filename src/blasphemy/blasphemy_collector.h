#pragma once

#include "fmt/xchar.h"

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace codesh::blasphemy
{

enum class blasphemy_type
{
    INIT,

    LEXICAL,
    SYNTAX,
    SEMANTIC,

    OUTPUT,

    UNKNOWN
};

struct line_info
{
    size_t line;
    size_t column;
};
struct code_position
{
    std::filesystem::path relative_source_path;
    line_info target_line;
};

struct blasphemy_info
{
    std::string details;
    blasphemy_type type;

    std::optional<code_position> code_pos;

    bool is_fatal;
};


class blasphemy_collector
{
public:
    /**
     * Adds a new blasphemy
     * @param details The message to display
     * @param type The blasphemy type
     * @param code_pos The location in the source code where the error was initiated from
     * @param is_fatal Whether the error is so bad such as it should immediately cease the compiler's flow
     */
    void add_blasphemy(std::string details, blasphemy_type type, std::optional<code_position> code_pos = std::nullopt,
        bool is_fatal = false);

    /**
     * Whether any error exists
     */
    [[nodiscard]] bool has_errors() const;

    /**
     * Prints all errors
     */
    void print_all_blasphemies() const;

private:
    std::vector<blasphemy_info> blasphemies;

    static std::string type_to_string(blasphemy_type type);

    [[nodiscard]] static std::string get_blasphemy_message(blasphemy_type type);

    [[nodiscard]] static fmt::format_string<std::string> get_random_message();

};


blasphemy_collector &get_blasphemy_collector();

}
