#pragma once

#include "../../output/jvm_target/class_file_builder.h"
#include "scope.h"

#include <list>
#include <stack>

namespace codesh::semantic_analyzer
{

class entry
{
    const std::vector<output::jvm_target::access_flag> access_flags;

public:
    explicit entry(const std::vector<output::jvm_target::access_flag> &access_flags);
    virtual ~entry();

    bool operator==(const entry &other) const;

    [[nodiscard]] const std::vector<output::jvm_target::access_flag> &get_access_flags() const;

    [[nodiscard]] virtual std::string get_full_descriptor() const = 0;
};


class type_entry final : public entry
{
    const std::string descriptor;

    // Allowed entries:
    // - methods
    // - fields (variables)
    std::list<std::unique_ptr<entry>> entries;

public:
    explicit type_entry(const std::vector<output::jvm_target::access_flag> &access_flags,
            std::string descriptor);

    [[nodiscard]] std::string get_full_descriptor() const override;

    [[nodiscard]] const std::list<std::unique_ptr<entry>> &get_entries() const;
    [[nodiscard]] std::list<std::unique_ptr<entry>> &get_entries();
};

class variable_entry final : public entry
{
    const std::string type_descriptor;

public:
    variable_entry(const std::vector<output::jvm_target::access_flag> &access_flags,
            std::string type_descriptor);

    [[nodiscard]] std::string get_full_descriptor() const override;
};

class method_entry final : public entry
{
    const std::vector<std::string> parameter_descriptors;
    const std::string return_type_descriptor;

    // Allowed entries:
    // - variables
    std::stack<std::unique_ptr<scope>> scopes;

public:
    method_entry(const std::vector<output::jvm_target::access_flag> &access_flags,
        std::vector<std::string> parameter_descriptors, std::string return_type_descriptor);

    [[nodiscard]] std::string get_full_descriptor() const override;

    [[nodiscard]] const std::vector<std::string> &get_parameter_descriptors() const;
    [[nodiscard]] std::string get_return_type_descriptor() const;

    [[nodiscard]] const std::stack<std::unique_ptr<scope>> &get_scopes() const;
    [[nodiscard]] std::stack<std::unique_ptr<scope>> &get_scopes();
};

}
