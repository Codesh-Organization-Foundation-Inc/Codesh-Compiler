#include "symbol.h"

#include <utility>

codesh::semantic_analyzer::symbol::symbol(const symbol_type symbol_type) :
    _symbol_type(symbol_type)
{
}

codesh::semantic_analyzer::symbol::~symbol() = default;

codesh::semantic_analyzer::symbol_type codesh::semantic_analyzer::symbol::get_symbol_type() const
{
    return _symbol_type;
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

codesh::semantic_analyzer::named_scope_map &codesh::semantic_analyzer::country_symbol::get_symbol_map()
{
    return scopes;
}

codesh::semantic_analyzer::country_symbol::country_symbol(ast::impl::ast_node *producing_node) :
    symbol(symbol_type::COUNTRY)
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

codesh::semantic_analyzer::named_scope_map &codesh::semantic_analyzer::type_symbol::get_symbol_map()
{
    return scopes;
}

codesh::semantic_analyzer::type_symbol::type_symbol(const std::vector<output::jvm_target::access_flag> &access_flags,
        ast::type_decl::type_declaration_ast_node &producing_node) :
    symbol(symbol_type::TYPE),
    i_ast_node_produced(producing_node),
    access_flags(access_flags)
{
}

const std::vector<codesh::output::jvm_target::access_flag> &codesh::semantic_analyzer::type_symbol::get_access_flags()
    const
{
    return access_flags;
}

codesh::semantic_analyzer::variable_symbol::variable_symbol(const symbol_type _symbol_type,
        std::unique_ptr<ast::type::type_ast_node> type) :
    symbol(_symbol_type),
    type(std::move(type))
{
}

codesh::semantic_analyzer::field_symbol::field_symbol(std::vector<output::jvm_target::access_flag> access_flags,
                                                      std::unique_ptr<ast::type::type_ast_node> type,
                                                      ast::impl::ast_node *producing_node) :
    variable_symbol(symbol_type::FIELD, std::move(type)),
    access_flags(std::move(access_flags))
{
}

const std::vector<codesh::output::jvm_target::access_flag> &codesh::semantic_analyzer::field_symbol::get_access_flags()
    const
{
    return access_flags;
}

codesh::semantic_analyzer::local_variable_symbol::local_variable_symbol(std::unique_ptr<ast::type::type_ast_node> type,
        ast::impl::ast_node *producing_node) :
    variable_symbol(symbol_type::LOCAL_VARIABLE, std::move(type))
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

codesh::semantic_analyzer::method_overloads_symbol::method_overloads_symbol() :
    symbol(symbol_type::METHOD_OVERLOADS)
{
}

const codesh::semantic_analyzer::named_scope_map &codesh::semantic_analyzer::method_overloads_symbol::get_symbol_map()
    const
{
    return scopes;
}

std::optional<std::reference_wrapper<codesh::semantic_analyzer::method_symbol>> codesh::semantic_analyzer::
    method_overloads_symbol::resolve_method(const std::string &name) const
{
    const auto result = resolve(name);

    if (!result.has_value())
        return std::nullopt;

    return *static_cast<method_symbol *>(&result.value().get()); // NOLINT(*-pro-type-static-cast-downcast)
}

codesh::semantic_analyzer::method_scope_symbol::method_scope_symbol(ast::impl::ast_node *producing_node) :
    symbol(symbol_type::METHOD_SCOPE)
{
}

codesh::semantic_analyzer::method_symbol::method_symbol(
         const std::vector<output::jvm_target::access_flag> &access_flags,
        std::vector<std::unique_ptr<ast::type::type_ast_node>> parameter_types,
        std::unique_ptr<ast::type::type_ast_node> return_type, ast::method_declaration_ast_node &producing_node) :
    symbol(symbol_type::METHOD),
    i_ast_node_produced(producing_node),
    access_flags(access_flags),
    parameter_types(std::move(parameter_types)),
    return_type(std::move(return_type))
{
}

const std::vector<codesh::output::jvm_target::access_flag> &codesh::semantic_analyzer::method_symbol::get_access_flags()
    const
{
    return access_flags;
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

const codesh::semantic_analyzer::method_scope_symbol &codesh::semantic_analyzer::method_symbol::get_scope() const
{
    return method_scope;
}

codesh::semantic_analyzer::method_scope_symbol &codesh::semantic_analyzer::method_symbol::get_scopes()
{
    return method_scope;
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
