#include "symbol.h"

#include <utility>

codesh::semantic_analyzer::symbol::symbol(symbol *const parent_symbol, const symbol_type symbol_type) :
    _symbol_type(symbol_type),
    parent_symbol(parent_symbol)
{
}

codesh::semantic_analyzer::symbol::~symbol() = default;

codesh::semantic_analyzer::symbol_type codesh::semantic_analyzer::symbol::get_symbol_type() const
{
    return _symbol_type;
}

codesh::semantic_analyzer::symbol *codesh::semantic_analyzer::symbol::get_parent_symbol() const
{
    return parent_symbol;
}

codesh::semantic_analyzer::i_scope_containing_symbol::~i_scope_containing_symbol() = default;

std::optional<std::reference_wrapper<codesh::semantic_analyzer::symbol>> codesh::semantic_analyzer::
    i_scope_containing_symbol::resolve(const std::string &name) const
{
    const auto result = get_symbol_map().find(name);

    if (result == get_symbol_map().end())
        return std::nullopt;

    return *result->second;
}

std::unique_ptr<codesh::semantic_analyzer::symbol> codesh::semantic_analyzer::i_scope_containing_symbol::
    resolve_and_move(const std::string &name)
{
    const auto result = get_symbol_map().find(name);

    if (result == get_symbol_map().end())
        return nullptr;

    std::unique_ptr<symbol> symbol = std::move(result->second);
    // This symbol is about to be moved out of this table -
    // so remove its duplicate entry from here.
    get_symbol_map().erase(result);

    return symbol;
}

void codesh::semantic_analyzer::i_scope_containing_symbol::remove_symbol(const std::string &name)
{
    get_symbol_map().erase(name);
}

const std::vector<codesh::semantic_analyzer::symbol_type> &codesh::semantic_analyzer::country_symbol::allowed_symbol_types()
    const
{
    return ALLOWED_SYMBOL_TYPES;
}

const codesh::semantic_analyzer::named_scope_map &codesh::semantic_analyzer::country_symbol::
    get_symbol_map() const
{
    return scopes;
}

const codesh::definition::fully_qualified_class_name &codesh::semantic_analyzer::country_symbol::get_full_name() const
{
    return full_name;
}

codesh::semantic_analyzer::named_scope_map &codesh::semantic_analyzer::country_symbol::get_symbol_map()
{
    return scopes;
}

codesh::semantic_analyzer::country_symbol::country_symbol(definition::fully_qualified_class_name full_name,
        symbol *const parent_symbol,
        ast::impl::ast_node *producing_node) :
    symbol(parent_symbol, symbol_type::COUNTRY),
    full_name(std::move(full_name))
{
}

const std::vector<codesh::semantic_analyzer::symbol_type> &codesh::semantic_analyzer::type_symbol::allowed_symbol_types() const
{
    return ALLOWED_SYMBOL_TYPES;
}

const codesh::semantic_analyzer::named_scope_map &codesh::semantic_analyzer::type_symbol::
    get_symbol_map() const
{
    return scopes;
}

codesh::ast::type_decl::type_declaration_ast_node *codesh::semantic_analyzer::type_symbol::get_producing_node() const
{
    return producing_node;
}

codesh::semantic_analyzer::named_scope_map &codesh::semantic_analyzer::type_symbol::get_symbol_map()
{
    return scopes;
}

codesh::semantic_analyzer::type_symbol::type_symbol(symbol *const parent_symbol,
        definition::fully_qualified_class_name full_name,
        const std::vector<output::jvm_target::access_flag> &access_flags,
        ast::type_decl::type_declaration_ast_node *producing_node) :
    symbol(parent_symbol, symbol_type::TYPE),
    full_name(std::move(full_name)),
    producing_node(producing_node),
    access_flags(access_flags)
{
}

const codesh::definition::fully_qualified_class_name &codesh::semantic_analyzer::type_symbol::get_full_name() const
{
    return full_name;
}

const std::vector<codesh::output::jvm_target::access_flag> &codesh::semantic_analyzer::type_symbol::get_access_flags()
    const
{
    return access_flags;
}

codesh::semantic_analyzer::variable_symbol::variable_symbol(symbol *const parent_symbol, const symbol_type _symbol_type,
                                                            std::unique_ptr<ast::type::type_ast_node> type)
    : symbol(parent_symbol, _symbol_type), type(std::move(type))
{
}

codesh::ast::type::type_ast_node *codesh::semantic_analyzer::variable_symbol::get_type() const
{
    return type.get();
}

codesh::semantic_analyzer::field_symbol::field_symbol(symbol *const parent_symbol,
                                                      std::vector<output::jvm_target::access_flag> access_flags,
                                                      std::unique_ptr<ast::type::type_ast_node> type,
                                                      ast::impl::ast_node *producing_node) :
    variable_symbol(parent_symbol, symbol_type::FIELD, std::move(type)),
    access_flags(std::move(access_flags))
{
}

const std::vector<codesh::output::jvm_target::access_flag> &codesh::semantic_analyzer::field_symbol::get_access_flags()
    const
{
    return access_flags;
}

codesh::semantic_analyzer::local_variable_symbol::local_variable_symbol(symbol *const parent_symbol,
        std::unique_ptr<ast::type::type_ast_node> type,
        ast::impl::ast_node *producing_node) :
    variable_symbol(parent_symbol, symbol_type::LOCAL_VARIABLE, std::move(type))
{
}

const std::vector<codesh::semantic_analyzer::symbol_type> &codesh::semantic_analyzer::method_overloads_symbol::
    allowed_symbol_types() const
{
    return ALLOWED_SYMBOL_TYPES;
}

codesh::semantic_analyzer::named_scope_map &codesh::semantic_analyzer::method_overloads_symbol::get_symbol_map()
{
    return scopes;
}

codesh::semantic_analyzer::method_overloads_symbol::method_overloads_symbol(symbol *const parent_symbol) :
    symbol(parent_symbol, symbol_type::METHOD_OVERLOADS)
{
}

const codesh::semantic_analyzer::named_scope_map &codesh::semantic_analyzer::method_overloads_symbol::get_symbol_map()
    const
{
    return scopes;
}

std::optional<std::reference_wrapper<codesh::semantic_analyzer::method_symbol>> codesh::semantic_analyzer::
    method_overloads_symbol::resolve_method(const std::string &params_descriptor) const
{
    const auto result = resolve(params_descriptor);

    if (!result.has_value())
        return std::nullopt;

    return *static_cast<method_symbol *>(&result.value().get()); // NOLINT(*-pro-type-static-cast-downcast)
}

codesh::semantic_analyzer::method_scope_symbol::method_scope_symbol(symbol *const parent_symbol,
        std::vector<std::reference_wrapper<local_variable_entry>> &index_to_local_variable,
        ast::impl::ast_node *producing_node) :
    symbol(parent_symbol, symbol_type::METHOD_SCOPE),
    index_to_local_variable(index_to_local_variable)
{
}

const std::unordered_map<std::string, std::unique_ptr<codesh::semantic_analyzer::local_variable_symbol>> &codesh::
    semantic_analyzer::method_scope_symbol::get_variables() const
{
    return local_variables;
}

size_t codesh::semantic_analyzer::method_scope_symbol::add_variable(std::string name,
        std::unique_ptr<local_variable_symbol> variable)
{
    const auto result = local_variables.emplace(std::move(name), std::move(variable));

    const size_t index = index_to_local_variable.size();
    index_to_local_variable.emplace_back(*result.first);

    return index;
}

const std::list<std::unique_ptr<codesh::semantic_analyzer::method_scope_symbol>> &codesh::semantic_analyzer::
    method_scope_symbol::get_inner_scopes() const
{
    return inner_method_scopes;
}

std::list<std::unique_ptr<codesh::semantic_analyzer::method_scope_symbol>> &codesh::semantic_analyzer::
    method_scope_symbol::get_inner_scopes()
{
    return inner_method_scopes;
}

codesh::semantic_analyzer::method_symbol::method_symbol(symbol *const parent_symbol, type_symbol &parent_type,
        std::unique_ptr<ast::type_decl::attributes_ast_node> attributes,
        std::vector<std::unique_ptr<ast::type::type_ast_node>> parameter_types,
        std::unique_ptr<ast::type::type_ast_node> return_type, ast::method::method_declaration_ast_node *producing_node) :
    symbol(parent_symbol, symbol_type::METHOD),
    attributes(std::move(attributes)),
    parameter_types(std::move(parameter_types)),
    return_type(std::move(return_type)),
    method_scope(this, local_variables),
    producing_node(producing_node),
    parent_type(parent_type)
{
}

std::unique_ptr<codesh::semantic_analyzer::method_scope_symbol> codesh::semantic_analyzer::method_symbol::
    create_method_scope(symbol &parent_scope)
{
    return std::make_unique<method_scope_symbol>(&parent_scope, local_variables);
}

const codesh::ast::type_decl::attributes_ast_node &codesh::semantic_analyzer::method_symbol::get_attributes() const
{
    return *attributes;
}

const std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> &codesh::semantic_analyzer::method_symbol::
    get_parameter_types() const
{
    return parameter_types;
}

codesh::ast::type::type_ast_node &codesh::semantic_analyzer::method_symbol::get_return_type() const
{
    return *return_type;
}

const std::vector<std::reference_wrapper<codesh::semantic_analyzer::local_variable_entry>> &codesh::semantic_analyzer::
    method_symbol::get_all_local_variables() const
{
    return local_variables;
}

const codesh::semantic_analyzer::method_scope_symbol &codesh::semantic_analyzer::method_symbol::get_scope() const
{
    return method_scope;
}

codesh::semantic_analyzer::method_scope_symbol &codesh::semantic_analyzer::method_symbol::get_scope()
{
    return method_scope;
}

codesh::ast::method::method_declaration_ast_node *codesh::semantic_analyzer::method_symbol::get_producing_node() const
{
    return producing_node;
}

codesh::semantic_analyzer::type_symbol &codesh::semantic_analyzer::method_symbol::get_parent_type() const
{
    return parent_type;
}

const std::vector<codesh::semantic_analyzer::symbol_type> codesh::semantic_analyzer::country_symbol::ALLOWED_SYMBOL_TYPES = {
    symbol_type::TYPE,
    symbol_type::COUNTRY
};

const std::vector<codesh::semantic_analyzer::symbol_type> codesh::semantic_analyzer::type_symbol::ALLOWED_SYMBOL_TYPES = {
    symbol_type::METHOD_OVERLOADS,
    symbol_type::FIELD
};

const std::vector<codesh::semantic_analyzer::symbol_type> codesh::semantic_analyzer::method_overloads_symbol::ALLOWED_SYMBOL_TYPES = {
    symbol_type::METHOD,
};
