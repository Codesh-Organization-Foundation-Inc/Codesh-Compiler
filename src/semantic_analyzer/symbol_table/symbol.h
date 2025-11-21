#pragma once

#include "../../output/jvm_target/class_file_builder.h"
#include "scope.h"

#include <list>
#include <optional>
#include <stack>
#include <unordered_map>

namespace codesh::semantic_analyzer
{

enum class symbol_type
{
    PACKAGE,
    TYPE,
    LOCAL_VARIABLE,
    FIELD,
    METHOD,
    METHOD_SCOPE
};

class symbol
{
    std::optional<std::reference_wrapper<symbol>> parent_symbol;
    symbol_type _symbol_type;

public:
    explicit symbol(symbol_type symbol_type);
    symbol(symbol *parent_symbol, symbol_type symbol_type);
    virtual ~symbol();

    [[nodiscard]] std::optional<std::reference_wrapper<symbol>> get_parent_symbol() const;
    [[nodiscard]] symbol_type get_symbol_type() const;
};


class i_scope_containing_symbol
{
protected:
    [[nodiscard]] virtual std::vector<symbol_type> allowed_symbol_types() const = 0;

    [[nodiscard]] virtual const named_scope_map &get_symbol_map() const = 0;
    [[nodiscard]] virtual named_scope_map &get_symbol_map() = 0;

public:
    virtual ~i_scope_containing_symbol();

    [[nodiscard]] std::optional<std::reference_wrapper<symbol>> resolve(const std::string &name) const;

    void add_symbol(std::string name, std::unique_ptr<symbol> entry);
};


class package_symbol final : public symbol, public i_scope_containing_symbol
{
    static const std::vector<symbol_type> ALLOWED_SYMBOL_TYPES;
    named_scope_map scopes;

protected:
    [[nodiscard]] std::vector<symbol_type> allowed_symbol_types() const override;

    [[nodiscard]] const named_scope_map &get_symbol_map() const override;
    [[nodiscard]] named_scope_map &get_symbol_map() override;

public:
    package_symbol();
    explicit package_symbol(package_symbol *parent_package);
};

class type_symbol final : public symbol, public i_scope_containing_symbol
{
    static const std::vector<symbol_type> ALLOWED_SYMBOL_TYPES;
    named_scope_map scopes;

    const std::string descriptor;
    const std::vector<output::jvm_target::access_flag> access_flags;

protected:
    [[nodiscard]] std::vector<symbol_type> allowed_symbol_types() const override;

    [[nodiscard]] const named_scope_map &get_symbol_map() const override;
    [[nodiscard]] named_scope_map &get_symbol_map() override;

public:
    type_symbol(symbol &parent_symbol, const std::vector<output::jvm_target::access_flag> &access_flags,
            std::string descriptor);

    [[nodiscard]] const std::vector<output::jvm_target::access_flag> &get_access_flags() const;
};


class variable_symbol : public symbol
{
    const std::string type_descriptor;

public:
    variable_symbol(symbol &parent_symbol, symbol_type _symbol_type, std::string type_descriptor);
};

class field_symbol final : public variable_symbol
{
    const std::vector<output::jvm_target::access_flag> access_flags;

public:
    field_symbol(symbol &parent_symbol, std::vector<output::jvm_target::access_flag> access_flags,
            std::string type_descriptor);

    [[nodiscard]] const std::vector<output::jvm_target::access_flag> &get_access_flags() const;
};

class local_variable_symbol final : public variable_symbol
{
public:
    local_variable_symbol(symbol &parent_symbol, std::string type_descriptor);
};


class method_scope_symbol final : public symbol
{
    std::unordered_map<std::string, std::unique_ptr<variable_symbol>> local_variables;
    std::list<std::unique_ptr<method_scope_symbol>> inner_method_scopes;

public:
    explicit method_scope_symbol(symbol &parent_symbol);
};

class method_symbol final : public symbol
{
    const std::vector<output::jvm_target::access_flag> access_flags;

    const std::vector<std::string> parameter_descriptors;
    const std::string return_type_descriptor;

    method_scope_symbol method_scope;

public:
    method_symbol(symbol &parent_symbol, const std::vector<output::jvm_target::access_flag> &access_flags,
        std::vector<std::string> parameter_descriptors, std::string return_type_descriptor);

    [[nodiscard]] const std::vector<output::jvm_target::access_flag> &get_access_flags() const;

    [[nodiscard]] const std::vector<std::string> &get_parameter_descriptors() const;
    [[nodiscard]] std::string get_return_type_descriptor() const;

    [[nodiscard]] const method_scope_symbol &get_scope() const;
    [[nodiscard]] method_scope_symbol &get_scopes();
};

}
