#pragma once

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

    [[nodiscard]] std::optional<std::string> parse_alias() const;

public:
    fully_qualified_name();

    // ReSharper disable once CppNonExplicitConvertingConstructor
    fully_qualified_name(const char *binary_fqn); // NOLINT(*-explicit-constructor)
    explicit fully_qualified_name(std::string part);

    /**
     * Clones this FQN and appends the provided part to it
     */
    [[nodiscard]] fully_qualified_name with(std::string part) const;
    [[nodiscard]] fully_qualified_name omit_last() const;


    void add(std::string part);
    [[nodiscard]] const std::vector<std::string> &get_parts() const;

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
