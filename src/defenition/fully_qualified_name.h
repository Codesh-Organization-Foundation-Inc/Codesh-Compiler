#pragma once

#include "lexer/source_file_info.h"

#include <optional>
#include <string>
#include <vector>

namespace codesh::definition
{

/**
 * Represents a Fully Qualified Class Name (FQN).
 *
 * For example, `מסוף ל־אמר`, `java.lang.String`, etc.
 */
class fully_qualified_name
{
    std::vector<std::string> parts;
    bool _is_wildcard;

    lexer::code_position code_position;

    [[nodiscard]] std::optional<std::string> parse_alias() const;

public:
    explicit fully_qualified_name(lexer::code_position code_position);
    fully_qualified_name(lexer::code_position code_position, std::string part);

    fully_qualified_name(lexer::code_position code_position,
            std::vector<std::string>::const_iterator name_start, std::vector<std::string>::const_iterator name_end);

    /**
     * Parses an FQN separated by slashes
     */
    static fully_qualified_name parse(const std::string &fqn_str, lexer::code_position code_position);


    [[nodiscard]] bool operator==(const fully_qualified_name &other) const;


    /**
     * Clones this FQN and appends the provided part to it
     */
    [[nodiscard]] fully_qualified_name with(std::string part) const;
    [[nodiscard]] fully_qualified_name omit_last() const;


    void add(std::string part);
    [[nodiscard]] const std::vector<std::string> &get_parts() const;

    void set_code_position(lexer::code_position pos);
    [[nodiscard]] lexer::code_position get_code_position() const;

    void set_is_wildcard(bool wildcard);
    [[nodiscard]] bool is_wildcard() const;


    [[nodiscard]] bool is_single_part() const;
    [[nodiscard]] std::string get_last_part() const;

    [[nodiscard]] std::string join(const std::string &sep = "/") const;
    /**
     * Joins strings with the ל־ seperator (the dot keyword)
     */
    [[nodiscard]] std::string holy_join() const;
};

}
