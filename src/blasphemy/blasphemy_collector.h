#pragma once

#include "fmt/xchar.h"
#include "lexer/source_file_info.h"

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

struct blasphemy_info
{
    std::string details;
    blasphemy_type type;

    std::optional<size_t> file_id;
    std::optional<lexer::code_position> code_pos;

    bool is_fatal;
};


class blasphemy_collector
{
    std::optional<size_t> file_id;
    std::filesystem::path source_directory_path;
    std::filesystem::path relative_source_path;

    std::vector<blasphemy_info> blasphemies;
    std::vector<blasphemy_info> warnings;

    static std::string type_to_string(blasphemy_type type);
    [[nodiscard]] static std::string get_blasphemy_message(blasphemy_type type);
    [[nodiscard]] static fmt::format_string<std::string> get_random_message();

    void print_blasphemy(const blasphemy_info &blasphemy, const std::string &color) const;

public:
    /**
     * Adds a new blasphemy
     * @param details The message to display
     * @param type The blasphemy type
     * @param code_pos The location in the source code where the error was initiated from
     * @param is_fatal Whether the error is so bad such as it should immediately cease the compiler's flow
     */
    void add_blasphemy(std::string details, blasphemy_type type, lexer::code_position code_pos, bool is_fatal = false);

    void add_warning(std::string details, blasphemy_type type, lexer::code_position code_pos);

    void set_source_directory(std::filesystem::path source_directory_path);

    /**
     * Makes all blasphemies from this point on blame the provided file.
     */
    void set_source_file(size_t file_id);
    /**
     * Makes all blasphemies from this point on blame the provided file.
     *
     * Use only when the file is not yet tokenized
     */
    void set_source_file(const std::filesystem::path &source_file_path);

    /**
     * Whether any error exists
     */
    [[nodiscard]] bool has_errors() const;


    // For LSP mostly
    [[nodiscard]] const std::vector<blasphemy_info> &get_all_blasphemies() const;
    [[nodiscard]] const std::vector<blasphemy_info> &get_all_warnings() const;

    void clear();


    /**
     * Prints all errors
     */
    void print_all_blasphemies() const;
};


blasphemy_collector &get_blasphemy_collector();

}
