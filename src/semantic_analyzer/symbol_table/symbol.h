#pragma once

#include "../../output/jvm_target/class_file_builder.h"
#include "../../parser/ast/type/type_ast_node.h"
#include "scope.h"

#include <list>
#include <optional>
#include <stack>
#include <unordered_map>

namespace codesh::semantic_analyzer
{

enum class symbol_type
{
    COUNTRY,
    TYPE,
    LOCAL_VARIABLE,
    FIELD,

    METHOD_OVERLOADS,
    METHOD,
    METHOD_SCOPE
};

class symbol
{
    symbol_type _symbol_type;

public:
    explicit symbol(symbol_type symbol_type);
    virtual ~symbol();

    [[nodiscard]] symbol_type get_symbol_type() const;
};


class i_scope_containing_symbol
{
protected:
    [[nodiscard]] virtual const std::vector<symbol_type> &allowed_symbol_types() const = 0;
    [[nodiscard]] virtual named_scope_map &get_symbol_map() = 0;

public:
    virtual ~i_scope_containing_symbol();

    [[nodiscard]] virtual const named_scope_map &get_symbol_map() const = 0;

    [[nodiscard]] std::optional<std::reference_wrapper<symbol>> resolve(const std::string &name) const;
    /**
     * Resolves the requested symbol. Returns `nullptr` in case not found.
     */
    [[nodiscard]] std::unique_ptr<symbol> resolve_and_move(const std::string &name);

    template <std::derived_from<symbol> T>
    std::pair<std::reference_wrapper<T>, bool> add_symbol(std::string name, std::unique_ptr<T> entry);
};


class country_symbol final : public symbol, public i_scope_containing_symbol
{
    static const std::vector<symbol_type> ALLOWED_SYMBOL_TYPES;
    named_scope_map scopes;

protected:
    [[nodiscard]] const std::vector<symbol_type> &allowed_symbol_types() const override;
    [[nodiscard]] named_scope_map &get_symbol_map() override;

public:
    country_symbol();

    [[nodiscard]] const named_scope_map &get_symbol_map() const override;
};

class type_symbol final : public symbol, public i_scope_containing_symbol
{
    static const std::vector<symbol_type> ALLOWED_SYMBOL_TYPES;
    named_scope_map scopes;

    //TODO: Add super type
    // const std::unique_ptr<ast::type::type_ast_node> super_type;
    const std::vector<output::jvm_target::access_flag> access_flags;

protected:
    [[nodiscard]] const std::vector<symbol_type> &allowed_symbol_types() const override;
    [[nodiscard]] named_scope_map &get_symbol_map() override;

public:
    explicit type_symbol(const std::vector<output::jvm_target::access_flag> &access_flags);

    [[nodiscard]] const std::vector<output::jvm_target::access_flag> &get_access_flags() const;

    [[nodiscard]] const named_scope_map &get_symbol_map() const override;
};


class variable_symbol : public symbol
{
    const std::unique_ptr<ast::type::type_ast_node> type;

public:
    variable_symbol(symbol_type _symbol_type, std::unique_ptr<ast::type::type_ast_node> type);
};

class field_symbol final : public variable_symbol
{
    const std::vector<output::jvm_target::access_flag> access_flags;

public:
    field_symbol(std::vector<output::jvm_target::access_flag> access_flags,
            std::unique_ptr<ast::type::type_ast_node> type);

    [[nodiscard]] const std::vector<output::jvm_target::access_flag> &get_access_flags() const;
};

class local_variable_symbol final : public variable_symbol
{
public:
    explicit local_variable_symbol(std::unique_ptr<ast::type::type_ast_node> type);
};


class method_overloads_symbol final : public symbol, public i_scope_containing_symbol
{
    static const std::vector<symbol_type> ALLOWED_SYMBOL_TYPES;
    // Maps parameter descriptors to method declaration
    named_scope_map scopes;

protected:
    [[nodiscard]] const std::vector<symbol_type> &allowed_symbol_types() const override;
    [[nodiscard]] named_scope_map &get_symbol_map() override;

public:
    method_overloads_symbol();

    [[nodiscard]] const named_scope_map &get_symbol_map() const override;
};


class method_scope_symbol final : public symbol
{
    std::unordered_map<std::string, std::unique_ptr<variable_symbol>> local_variables;
    std::list<std::unique_ptr<method_scope_symbol>> inner_method_scopes;

public:
    method_scope_symbol();
};

class method_symbol final : public symbol
{
    const std::vector<output::jvm_target::access_flag> access_flags;

    const std::vector<std::unique_ptr<ast::type::type_ast_node>> parameter_types;
    const std::unique_ptr<ast::type::type_ast_node> return_type;

    method_scope_symbol method_scope;

public:
    method_symbol(const std::vector<output::jvm_target::access_flag> &access_flags,
            std::vector<std::unique_ptr<ast::type::type_ast_node>> parameter_types,
            std::unique_ptr<ast::type::type_ast_node> return_type);

    [[nodiscard]] const std::vector<output::jvm_target::access_flag> &get_access_flags() const;

    [[nodiscard]] const std::vector<std::unique_ptr<ast::type::type_ast_node>> &get_parameter_types() const;
    [[nodiscard]] ast::type::type_ast_node &get_return_type() const;

    [[nodiscard]] const method_scope_symbol &get_scope() const;
    [[nodiscard]] method_scope_symbol &get_scopes();
};

}

#include "symbol.tpp"
