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
    //TODO:
    // if (producing_node != nullptr)
    // {
    //     producing_node->set_resolved(*this);
    // }
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

codesh::semantic_analyzer::variable_symbol::variable_symbol(symbol *parent_symbol, symbol_type _symbol_type,
                                                            std::unique_ptr<ast::type::type_ast_node> type) :
    symbol(parent_symbol, _symbol_type),
    type(std::move(type))
{
}

codesh::ast::type::type_ast_node *codesh::semantic_analyzer::variable_symbol::get_type() const
{
    return type.get();
}

codesh::semantic_analyzer::named_scope_map &codesh::semantic_analyzer::type_symbol::get_symbol_map()
{
    return scopes;
}

codesh::semantic_analyzer::type_symbol::type_symbol(symbol *const parent_symbol,
        definition::fully_qualified_class_name full_name,
        std::unique_ptr<ast::type_decl::attributes_ast_node> attributes,
        ast::type_decl::type_declaration_ast_node *producing_node) :
    symbol(parent_symbol, symbol_type::TYPE),
    full_name(std::move(full_name)),
    producing_node(producing_node),
    attributes(std::move(attributes))
{
    if (producing_node != nullptr)
    {
        producing_node->set_resolved(*this);
    }
}

const codesh::definition::fully_qualified_class_name &codesh::semantic_analyzer::type_symbol::get_full_name() const
{
    return full_name;
}

const codesh::ast::type_decl::attributes_ast_node &codesh::semantic_analyzer::type_symbol::get_attributes() const
{
    return *attributes;
}

codesh::semantic_analyzer::field_symbol::field_symbol(symbol *parent_symbol,
                                                      definition::fully_qualified_class_name full_name,
                                                      std::unique_ptr<ast::type_decl::attributes_ast_node> attributes,
                                                      std::unique_ptr<ast::type::type_ast_node> type,
                                                      ast::local_variable_declaration_ast_node *producing_node) :
    variable_symbol(parent_symbol, symbol_type::FIELD, std::move(type)),
    full_name(std::move(full_name)),
    attributes(std::move(attributes)),
    producing_node(producing_node)
{
}

codesh::ast::type_decl::attributes_ast_node &codesh::semantic_analyzer::field_symbol::get_attributes() const
{
    return *attributes;
}

const codesh::definition::fully_qualified_class_name &codesh::semantic_analyzer::field_symbol::get_full_name() const
{
    return full_name;
}

codesh::ast::local_variable_declaration_ast_node *codesh::semantic_analyzer::field_symbol::get_producing_node() const
{
    return producing_node;
}

codesh::semantic_analyzer::local_variable_symbol::local_variable_symbol(symbol *const parent_symbol,
        std::unique_ptr<ast::type::type_ast_node> type,
        ast::local_variable_declaration_ast_node *producing_node) :
    variable_symbol(parent_symbol, symbol_type::LOCAL_VARIABLE, std::move(type)),
    producing_node(producing_node)
{
    if (producing_node != nullptr)
    {
        producing_node->set_resolved(*this);
    }
}

codesh::ast::local_variable_declaration_ast_node *codesh::semantic_analyzer::local_variable_symbol::get_producing_node()
    const
{
    return producing_node;
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
        ast::method::method_scope_ast_node *producing_node) :
    symbol(parent_symbol, symbol_type::METHOD_SCOPE),
    producing_node(producing_node),
    index_to_local_variable(index_to_local_variable)
{
    if (producing_node != nullptr)
    {
        producing_node->set_resolved(*this);
    }
}

const std::unordered_map<std::string, std::unique_ptr<codesh::semantic_analyzer::local_variable_symbol>> &codesh::
    semantic_analyzer::method_scope_symbol::get_variables() const
{
    return local_variables;
}

codesh::ast::method::method_scope_ast_node *codesh::semantic_analyzer::method_scope_symbol::get_producing_node() const
{
    return producing_node;
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
        definition::fully_qualified_class_name full_name,
        std::unique_ptr<ast::type_decl::attributes_ast_node> attributes,
        std::vector<std::unique_ptr<ast::type::type_ast_node>> parameter_types,
        std::unique_ptr<ast::type::type_ast_node> return_type, ast::method::method_declaration_ast_node *producing_node) :
    symbol(parent_symbol, symbol_type::METHOD),
    full_name(std::move(full_name)),
    attributes(std::move(attributes)),
    parameter_types(std::move(parameter_types)),
    return_type(std::move(return_type)),
    method_scope(this, local_variables, producing_node ? &producing_node->get_method_scope() : nullptr),
    producing_node(producing_node),
    parent_type(parent_type)
{
    if (producing_node != nullptr)
    {
        producing_node->set_resolved(*this);
    }
}

std::unique_ptr<codesh::semantic_analyzer::method_scope_symbol> codesh::semantic_analyzer::method_symbol::
    create_method_scope(symbol &parent_scope)
{
    return std::make_unique<method_scope_symbol>(&parent_scope, local_variables);
}

const codesh::definition::fully_qualified_class_name &codesh::semantic_analyzer::method_symbol::get_full_name() const
{
    return full_name;
}

void codesh::semantic_analyzer::method_symbol::set_full_name(definition::fully_qualified_class_name name)
{
    full_name = std::move(name);
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
