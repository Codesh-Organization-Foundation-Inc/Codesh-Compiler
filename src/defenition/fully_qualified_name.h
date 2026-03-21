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

    lexer::code_range source_range;

    [[nodiscard]] std::optional<std::string> parse_alias() const;

public:
    explicit fully_qualified_name(lexer::code_position start);
    fully_qualified_name(lexer::code_position start, std::string part);

    fully_qualified_name(lexer::code_position start,
            std::vector<std::string>::const_iterator name_start, std::vector<std::string>::const_iterator name_end);

    /**
     * Parses a binary FQN (FQN separated by slashes)
     */
    static fully_qualified_name parse(const std::string &fqn_str, lexer::code_position start);


    [[nodiscard]] bool operator==(const fully_qualified_name &other) const;


    /**
     * Clones this FQN and appends the provided part to it
     */
    [[nodiscard]] fully_qualified_name with(std::string part) const;
    [[nodiscard]] fully_qualified_name omit_last() const;


    void add(std::string part);
    [[nodiscard]] const std::vector<std::string> &get_parts() const;

    void set_start_position(lexer::code_position pos);
    void set_end_position(lexer::code_position pos);
    [[nodiscard]] const lexer::code_range &get_source_range() const;

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

struct fully_qualified_name_hasher
{
    size_t operator()(const fully_qualified_name &fqn) const noexcept;
};

}
