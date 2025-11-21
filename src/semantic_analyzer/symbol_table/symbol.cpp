#include "symbol.h"

#include "symbol_table.h"

#include <utility>

codesh::semantic_analyzer::symbol::symbol(const symbol_type symbol_type) :
    parent_symbol(std::nullopt),
    _symbol_type(symbol_type)
{
}

codesh::semantic_analyzer::symbol::symbol(symbol *const parent_symbol, const symbol_type symbol_type) :
    parent_symbol(*parent_symbol),
    _symbol_type(symbol_type)
{
}

codesh::semantic_analyzer::symbol::~symbol() = default;

std::optional<std::reference_wrapper<codesh::semantic_analyzer::symbol>> codesh::semantic_analyzer::symbol::
    get_parent_symbol() const
{
    return parent_symbol;
}

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

bool codesh::semantic_analyzer::i_scope_containing_symbol::add_symbol(std::string name, std::unique_ptr<symbol> entry)
{
     for (const symbol_type type : allowed_symbol_types())
     {
         if (type != entry->get_symbol_type())
             throw std::runtime_error("Illegal symbol entry");
     }

    const auto [it, added] = get_symbol_map().emplace(std::move(name), std::move(entry));
    return added;
}

std::vector<codesh::semantic_analyzer::symbol_type> codesh::semantic_analyzer::country_symbol::allowed_symbol_types()
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

codesh::semantic_analyzer::country_symbol::country_symbol() : symbol(symbol_type::COUNTRY)
{
}

codesh::semantic_analyzer::country_symbol::country_symbol(country_symbol *parent_package) :
    symbol(parent_package, symbol_type::COUNTRY)
{
}

std::vector<codesh::semantic_analyzer::symbol_type> codesh::semantic_analyzer::type_symbol::allowed_symbol_types() const
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

codesh::semantic_analyzer::type_symbol::type_symbol(symbol &parent_symbol,
                                                    const std::vector<output::jvm_target::access_flag> &access_flags,
                                                    std::string descriptor) :
    symbol(&parent_symbol, symbol_type::TYPE),
    descriptor(std::move(descriptor)),
    access_flags(access_flags)
{
}

const std::vector<codesh::output::jvm_target::access_flag> &codesh::semantic_analyzer::type_symbol::get_access_flags()
    const
{
    return access_flags;
}

codesh::semantic_analyzer::variable_symbol::variable_symbol(symbol &parent_symbol, const symbol_type _symbol_type,
        std::string type_descriptor) :
    symbol(&parent_symbol, _symbol_type),
    type_descriptor(std::move(type_descriptor))
{
}

codesh::semantic_analyzer::field_symbol::field_symbol(symbol &parent_symbol,
                                                      std::vector<output::jvm_target::access_flag> access_flags,
                                                      std::string type_descriptor) :
    variable_symbol(parent_symbol, symbol_type::FIELD, std::move(type_descriptor)),
    access_flags(std::move(access_flags))
{
}

const std::vector<codesh::output::jvm_target::access_flag> &codesh::semantic_analyzer::field_symbol::get_access_flags()
    const
{
    return access_flags;
}

codesh::semantic_analyzer::local_variable_symbol::local_variable_symbol(symbol &parent_symbol,
                                                                        std::string type_descriptor) :
    variable_symbol(parent_symbol, symbol_type::LOCAL_VARIABLE, std::move(type_descriptor))
{
}

codesh::semantic_analyzer::method_scope_symbol::method_scope_symbol(symbol &parent_symbol) :
    symbol(&parent_symbol, symbol_type::METHOD_SCOPE)
{
}

codesh::semantic_analyzer::method_symbol::method_symbol(
        symbol &parent_symbol, const std::vector<output::jvm_target::access_flag> &access_flags,
        std::vector<std::string> parameter_descriptors, std::string return_type_descriptor) :
    symbol(&parent_symbol, symbol_type::METHOD),
    access_flags(access_flags),
    parameter_descriptors(std::move(parameter_descriptors)),
    return_type_descriptor(std::move(return_type_descriptor)),

    method_scope(*this)
{
}

const std::vector<codesh::output::jvm_target::access_flag> &codesh::semantic_analyzer::method_symbol::get_access_flags()
    const
{
    return access_flags;
}

const std::vector<std::string> &codesh::semantic_analyzer::method_symbol::get_parameter_descriptors() const
{
    return parameter_descriptors;
}

std::string codesh::semantic_analyzer::method_symbol::get_return_type_descriptor() const
{
    return return_type_descriptor;
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
    symbol_type::METHOD,
    symbol_type::FIELD
};
