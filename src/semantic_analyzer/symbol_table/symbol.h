#pragma once

#include "../../output/jvm_target/class_file_builder.h"
#include "../../parser/ast/type/type_ast_node.h"
#include "../../parser/ast/type_declaration/type_declaration_ast_node.h"
#include "../../parser/ast/method/method_declaration_ast_node.h"
#include "scope.h"

#include <list>
#include <optional>
#include <unordered_map>

namespace codesh::semantic_analyzer
{
class method_symbol;


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
    symbol *const parent_symbol;

public:
    symbol(symbol *parent_symbol, symbol_type symbol_type);
    virtual ~symbol();

    [[nodiscard]] symbol_type get_symbol_type() const;
    [[nodiscard]] symbol *get_parent_symbol() const;
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
    void remove_symbol(const std::string &name);
};

/**
 * @tparam T The producing node type. Must extend @link ast_node \endlink.
 */
template <typename T>
class i_resolvable_symbol
{
public:
    virtual ~i_resolvable_symbol();

    [[nodiscard]] virtual T *get_producing_node() const = 0;
    [[nodiscard]] virtual const definition::fully_qualified_class_name &get_full_name() const = 0;
};


//TODO: Attach ast node
class country_symbol final : public symbol, public i_scope_containing_symbol
{
    const definition::fully_qualified_class_name full_name;

    static const std::vector<symbol_type> ALLOWED_SYMBOL_TYPES;
    named_scope_map scopes;

protected:
    [[nodiscard]] const std::vector<symbol_type> &allowed_symbol_types() const override;
    [[nodiscard]] named_scope_map &get_symbol_map() override;

public:
    explicit country_symbol(definition::fully_qualified_class_name full_name, symbol *parent_symbol = nullptr,
            ast::impl::ast_node *producing_node = nullptr);

    [[nodiscard]] const named_scope_map &get_symbol_map() const override;

    [[nodiscard]] const definition::fully_qualified_class_name &get_full_name() const;
};

class type_symbol final : public symbol, public i_scope_containing_symbol,
        public i_resolvable_symbol<ast::type_decl::type_declaration_ast_node>
{
    const definition::fully_qualified_class_name full_name;

    static const std::vector<symbol_type> ALLOWED_SYMBOL_TYPES;
    named_scope_map scopes;


    ast::type_decl::type_declaration_ast_node *producing_node;

    //TODO: Add super type
    // const std::unique_ptr<ast::type::type_ast_node> super_type;
    const std::unique_ptr<ast::type_decl::attributes_ast_node> attributes;

protected:
    [[nodiscard]] const std::vector<symbol_type> &allowed_symbol_types() const override;
    [[nodiscard]] named_scope_map &get_symbol_map() override;

public:
    type_symbol(symbol *parent_symbol, definition::fully_qualified_class_name full_name,
            std::unique_ptr<ast::type_decl::attributes_ast_node> attributes,
            ast::type_decl::type_declaration_ast_node *producing_node);

    [[nodiscard]] const definition::fully_qualified_class_name &get_full_name() const override;

    [[nodiscard]] const ast::type_decl::attributes_ast_node &get_attributes() const;

    [[nodiscard]] const named_scope_map &get_symbol_map() const override;
    [[nodiscard]] ast::type_decl::type_declaration_ast_node *get_producing_node() const override;
};


class variable_symbol : public symbol
{
    const std::unique_ptr<ast::type::type_ast_node> type;

public:
    variable_symbol(symbol *parent_symbol, symbol_type _symbol_type, std::unique_ptr<ast::type::type_ast_node> type);

    [[nodiscard]] ast::type::type_ast_node *get_type() const;
};

//TODO: Make resolvable
class field_symbol final : public variable_symbol
{
    const definition::fully_qualified_class_name full_name;
    const std::unique_ptr<ast::type_decl::attributes_ast_node> attributes;

public:
    field_symbol(symbol *parent_symbol, definition::fully_qualified_class_name full_name,
            std::unique_ptr<ast::type_decl::attributes_ast_node> attributes,
            std::unique_ptr<ast::type::type_ast_node> type, ast::impl::ast_node *producing_node = nullptr);

    [[nodiscard]] ast::type_decl::attributes_ast_node &get_attributes() const;
    [[nodiscard]] const definition::fully_qualified_class_name &get_full_name() const;
};

class local_variable_symbol final : public variable_symbol,
    public i_resolvable_symbol<ast::variable_declaration_ast_node>
{
    definition::fully_qualified_class_name full_name;
    ast::variable_declaration_ast_node *producing_node;

public:
    explicit local_variable_symbol(symbol *parent_symbol,
            definition::fully_qualified_class_name full_name, std::unique_ptr<ast::type::type_ast_node> type,
            ast::variable_declaration_ast_node *producing_node = nullptr);

    [[nodiscard]] ast::variable_declaration_ast_node *get_producing_node() const override;
    [[nodiscard]] const definition::fully_qualified_class_name &get_full_name() const override;
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
    explicit method_overloads_symbol(symbol *parent_symbol);

    [[nodiscard]] const named_scope_map &get_symbol_map() const override;

    [[nodiscard]] std::optional<std::reference_wrapper<method_symbol>> resolve_method(
            const std::string &params_descriptor) const;
};


using local_variable_entry = std::pair<const std::string, std::unique_ptr<local_variable_symbol>>;

//TODO: Handle producing node
class method_scope_symbol final : public symbol
{
    std::unordered_map<std::string, std::unique_ptr<local_variable_symbol>> local_variables;
    std::vector<std::reference_wrapper<local_variable_entry>> &index_to_local_variable;

    std::list<std::unique_ptr<method_scope_symbol>> inner_method_scopes;

public:
    method_scope_symbol(symbol *parent_symbol,
            std::vector<std::reference_wrapper<local_variable_entry>> &index_to_local_variable,
            ast::impl::ast_node *producing_node = nullptr);

    [[nodiscard]] const std::unordered_map<std::string, std::unique_ptr<local_variable_symbol>> &get_variables() const;

    /**
     * @returns The new variable's index
     */
    size_t add_variable(std::string name, std::unique_ptr<local_variable_symbol> variable);

    [[nodiscard]] const std::list<std::unique_ptr<method_scope_symbol>> &get_inner_scopes() const;
    [[nodiscard]] std::list<std::unique_ptr<method_scope_symbol>> &get_inner_scopes();
};

class method_symbol final : public symbol, public i_resolvable_symbol<ast::method::method_declaration_ast_node>
{
    definition::fully_qualified_class_name full_name;

    const std::unique_ptr<ast::type_decl::attributes_ast_node> attributes;

    const std::vector<std::unique_ptr<ast::type::type_ast_node>> parameter_types;
    const std::unique_ptr<ast::type::type_ast_node> return_type;

    std::vector<std::reference_wrapper<local_variable_entry>> local_variables;
    method_scope_symbol method_scope;

    ast::method::method_declaration_ast_node *producing_node;
    type_symbol &parent_type;

public:
    method_symbol(symbol *parent_symbol, type_symbol &parent_type,
            definition::fully_qualified_class_name full_name,
            std::unique_ptr<ast::type_decl::attributes_ast_node> attributes,
            std::vector<std::unique_ptr<ast::type::type_ast_node>> parameter_types,
            std::unique_ptr<ast::type::type_ast_node> return_type,
            ast::method::method_declaration_ast_node *producing_node);

    [[nodiscard]] std::unique_ptr<method_scope_symbol> create_method_scope(symbol &parent_scope);

    [[nodiscard]] const definition::fully_qualified_class_name &get_full_name() const override;
    void set_full_name (definition::fully_qualified_class_name name);


    [[nodiscard]] const ast::type_decl::attributes_ast_node &get_attributes() const;

    [[nodiscard]] const std::vector<std::unique_ptr<ast::type::type_ast_node>> &get_parameter_types() const;
    [[nodiscard]] ast::type::type_ast_node &get_return_type() const;

    [[nodiscard]] const std::vector<std::reference_wrapper<local_variable_entry>> &get_all_local_variables() const;

    [[nodiscard]] const method_scope_symbol &get_scope() const;
    [[nodiscard]] method_scope_symbol &get_scope();

    [[nodiscard]] ast::method::method_declaration_ast_node *get_producing_node() const override;


    [[nodiscard]] type_symbol &get_parent_type() const;
};

}

#include "symbol.tpp"
