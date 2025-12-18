#pragma once

#include <list>
#include <string>

namespace codesh::definition
{

/**
 * Represents a Fully Qualified Class Name (FQCN).
 *
 * For example, `מסוף ל־אמר`, `java.lang.String`, etc.
 */
class fully_qualified_class_name
{
    std::list<std::string> parts;
    bool _is_wildcard;

public:
    fully_qualified_class_name();
    // ReSharper disable once CppNonExplicitConvertingConstructor

    fully_qualified_class_name(const char *binary_fqcn); // NOLINT(*-explicit-constructor)
    explicit fully_qualified_class_name(std::string part);


    void add(std::string part);
    [[nodiscard]] const std::list<std::string> &get_parts() const;

    void set_is_wildcard(bool wildcard);
    [[nodiscard]] bool is_wildcard() const;

    [[nodiscard]] bool is_single_part() const;
    [[nodiscard]] std::string get_last_part() const;

    [[nodiscard]] std::string join(char sep = '/') const;
};

}
