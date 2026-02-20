#pragma once

#include "defenition/fully_qualified_name.h"
#include "parser/ast/type/type_ast_node.h"
#include "parser/ast/type_declaration/attributes_ast_node.h"
#include "scope.h"
#include "symbol.h"
#include "symbol_type.h"

#include <map>
#include <optional>

namespace codesh::semantic_analyzer
{
class method_symbol;
class i_scope_containing_symbol;
}

namespace codesh::ast
{
class local_variable_declaration_ast_node;
}
namespace codesh::ast::method
{
class method_scope_ast_node;
class method_declaration_ast_node;
}
namespace codesh::ast::type_decl
{
class type_declaration_ast_node;
}

namespace codesh::semantic_analyzer
{
class type_symbol;


class symbol
{
    symbol_type _symbol_type;
    i_scope_containing_symbol *const parent_symbol;

public:
    symbol(i_scope_containing_symbol *parent_symbol, symbol_type symbol_type);
    virtual ~symbol();

    [[nodiscard]] symbol_type get_symbol_type() const;
    [[nodiscard]] i_scope_containing_symbol *get_parent_symbol() const;
};


class i_scope_containing_symbol
{
protected:
    [[nodiscard]] virtual symbols_collection &get_scope() = 0;

public:
    virtual ~i_scope_containing_symbol();

    [[nodiscard]] virtual const symbols_collection &get_scope() const = 0;

    /**
     * Attempts to find the symbol named `name`, and returns it.
     * Upon failure, consults the parent symbol.
     */
    [[nodiscard]] std::optional<std::reference_wrapper<symbol>> resolve_up(const std::string &name) const;
};

/**
 * @tparam T The producing node type. Must extend @link i_symbolically_linked \endlink.
 */
template <typename T>
class i_ast_produced
{
public:
    virtual ~i_ast_produced();

    [[nodiscard]] virtual T *get_producing_node() const = 0;
};

/**
 * @tparam T The producing node type. Must extend @link i_resolvable \endlink.
 */
template <typename T>
class i_resolvable_symbol : public i_ast_produced<T>
{
public:
    [[nodiscard]] virtual const definition::fully_qualified_name &get_full_name() const = 0;
};


//TODO: Attach ast node
class country_symbol final : public symbol, public i_scope_containing_symbol
{
    const definition::fully_qualified_name full_name;

    static const std::vector<symbol_type> ALLOWED_SYMBOL_TYPES;
    named_symbol_map scope;

public:
    explicit country_symbol(definition::fully_qualified_name full_name,
            i_scope_containing_symbol *parent_symbol = nullptr, ast::impl::ast_node *producing_node = nullptr);

    [[nodiscard]] named_symbol_map &get_scope() override;
    [[nodiscard]] const named_symbol_map &get_scope() const override;

    [[nodiscard]] const definition::fully_qualified_name &get_full_name() const;
};

class type_symbol final : public symbol, public i_scope_containing_symbol,
        public i_resolvable_symbol<ast::type_decl::type_declaration_ast_node>
{
    const definition::fully_qualified_name full_name;

    static const std::vector<symbol_type> ALLOWED_SYMBOL_TYPES;
    named_symbol_map scope;


    ast::type_decl::type_declaration_ast_node *producing_node;

    type_symbol *super_type = nullptr;
    std::vector<type_symbol *> interfaces;

    const std::unique_ptr<ast::type_decl::attributes_ast_node> attributes;

public:
    type_symbol(i_scope_containing_symbol *parent_symbol, definition::fully_qualified_name full_name,
            std::unique_ptr<ast::type_decl::attributes_ast_node> attributes,
            ast::type_decl::type_declaration_ast_node *producing_node);

    [[nodiscard]] const definition::fully_qualified_name &get_full_name() const override;

    [[nodiscard]] const ast::type_decl::attributes_ast_node &get_attributes() const;

    [[nodiscard]] type_symbol *get_super_type() const;
    void set_super_type(type_symbol *super_type);

    [[nodiscard]] const std::vector<type_symbol *> &get_interfaces() const;
    void add_interface(type_symbol *interface_symbol);

    [[nodiscard]] named_symbol_map &get_scope() override;
    [[nodiscard]] const named_symbol_map &get_scope() const override;

    [[nodiscard]] ast::type_decl::type_declaration_ast_node *get_producing_node() const override;
};

class variable_symbol : public symbol
{
    const std::unique_ptr<ast::type::type_ast_node> type;

public:
    variable_symbol(i_scope_containing_symbol *parent_symbol, symbol_type _symbol_type, std::unique_ptr<ast::type::type_ast_node> type);

    [[nodiscard]] ast::type::type_ast_node *get_type() const;

    [[nodiscard]] virtual ast::local_variable_declaration_ast_node *get_producing_node() const = 0;
};

class field_symbol final : public variable_symbol, public i_resolvable_symbol<ast::local_variable_declaration_ast_node>
{
    const definition::fully_qualified_name full_name;
    const std::unique_ptr<ast::type_decl::attributes_ast_node> attributes;

    ast::local_variable_declaration_ast_node *producing_node;

public:
    field_symbol(i_scope_containing_symbol *parent_symbol, definition::fully_qualified_name full_name,
            std::unique_ptr<ast::type_decl::attributes_ast_node> attributes,
            std::unique_ptr<ast::type::type_ast_node> type,
            ast::local_variable_declaration_ast_node *producing_node = nullptr);

    [[nodiscard]] ast::type_decl::attributes_ast_node &get_attributes() const;
    [[nodiscard]] const definition::fully_qualified_name &get_full_name() const override;

    [[nodiscard]] ast::local_variable_declaration_ast_node *get_producing_node() const override;
};

class local_variable_symbol final : public variable_symbol,
    public i_ast_produced<ast::local_variable_declaration_ast_node>
{
    ast::local_variable_declaration_ast_node *producing_node;
    const size_t index;

public:
    local_variable_symbol(i_scope_containing_symbol *parent_symbol, std::unique_ptr<ast::type::type_ast_node> type,
            size_t index, ast::local_variable_declaration_ast_node *producing_node = nullptr);

    [[nodiscard]] ast::local_variable_declaration_ast_node *get_producing_node() const override;
    /**
     * Returns the index of the variable as per the JVM specifications.
     *
     * That means that long and double types occupy 2 slots. This affects later slots as well.
     *
     * @return The index of the variable as per the JVM specifications.
     */
    [[nodiscard]] int get_jvm_index() const;
};


class method_overloads_symbol final : public symbol, public i_scope_containing_symbol
{
    static const std::vector<symbol_type> ALLOWED_SYMBOL_TYPES;
    named_symbol_map scope;

public:
    explicit method_overloads_symbol(i_scope_containing_symbol *parent_symbol);

    [[nodiscard]] named_symbol_map &get_scope() override;
    [[nodiscard]] const named_symbol_map &get_scope() const override;

    [[nodiscard]] std::optional<std::reference_wrapper<method_symbol>> resolve_method(
            const std::string &params_descriptor) const;
};


struct indexed_locals_container
{
    std::map<std::string, std::reference_wrapper<local_variable_symbol>> name_to_var;
    size_t slots_used;
};

class method_scope_symbol final : public symbol, public i_ast_produced<ast::method::method_scope_ast_node>,
    public i_scope_containing_symbol
{
    static const std::vector<symbol_type> ALLOWED_SYMBOL_TYPES;

    ast::method::method_scope_ast_node *producing_node;

    indexed_locals_container &index_to_local_variable;

    named_symbol_map scope;
    std::vector<std::unique_ptr<method_scope_symbol>> inner_scopes;

protected:
    [[nodiscard]] named_symbol_map &get_scope() override;

public:
    method_scope_symbol(i_scope_containing_symbol *parent_symbol, indexed_locals_container &index_to_local_variable,
            ast::method::method_scope_ast_node *producing_node = nullptr);

    [[nodiscard]] ast::method::method_scope_ast_node *get_producing_node() const override;

    /**
     * @returns The new variable's index
     */
    size_t add_variable(const std::string &name, std::unique_ptr<local_variable_symbol> variable);
    /**
     * @returns The new variable's index
     */
    size_t add_variable(ast::local_variable_declaration_ast_node &variable);

    [[nodiscard]] const named_symbol_map &get_scope() const override;

    method_scope_symbol &add_inner_scope(std::unique_ptr<method_scope_symbol> method_scope);
};

class method_symbol final : public symbol, public i_resolvable_symbol<ast::method::method_declaration_ast_node>,
    public i_scope_containing_symbol
{
    definition::fully_qualified_name full_name;

    const std::unique_ptr<ast::type_decl::attributes_ast_node> attributes;

    const std::vector<std::unique_ptr<ast::type::type_ast_node>> parameter_types;
    const std::unique_ptr<ast::type::type_ast_node> return_type;

    indexed_locals_container local_variables;

    static const std::vector<symbol_type> ALLOWED_SYMBOL_TYPES;
    symbol_list scope;
    method_scope_symbol *method_scope;

    ast::method::method_declaration_ast_node *producing_node;
    type_symbol &parent_type;

protected:
    [[nodiscard]] symbols_collection &get_scope() override;

public:
    method_symbol(i_scope_containing_symbol *parent_symbol, type_symbol &parent_type,
            definition::fully_qualified_name full_name,
            std::unique_ptr<ast::type_decl::attributes_ast_node> attributes,
            std::vector<std::unique_ptr<ast::type::type_ast_node>> parameter_types,
            std::unique_ptr<ast::type::type_ast_node> return_type,
            ast::method::method_declaration_ast_node *producing_node);

    [[nodiscard]] std::unique_ptr<method_scope_symbol> create_method_scope(i_scope_containing_symbol &parent_scope,
            ast::method::method_scope_ast_node &producing_node);

    [[nodiscard]] const definition::fully_qualified_name &get_full_name() const override;
    void set_full_name(definition::fully_qualified_name name);


    [[nodiscard]] const ast::type_decl::attributes_ast_node &get_attributes() const;

    [[nodiscard]] const std::vector<std::unique_ptr<ast::type::type_ast_node>> &get_parameter_types() const;
    [[nodiscard]] ast::type::type_ast_node &get_return_type() const;

    [[nodiscard]] const indexed_locals_container &get_all_local_variables() const;

    [[nodiscard]] method_scope_symbol &get_method_scope() const;
    [[nodiscard]] const symbol_list &get_scope() const override;

    [[nodiscard]] ast::method::method_declaration_ast_node *get_producing_node() const override;


    [[nodiscard]] type_symbol &get_parent_type() const;
};

}

#include "symbol.tpp"
