#include "symbol_table_entry.h"

#include <utility>

codesh::semantic_analyzer::type_entry::type_entry(const std::vector<output::jvm_target::access_flag> &access_flags,
        std::string descriptor) :
    entry(access_flags),
    descriptor(std::move(descriptor))
{
}

codesh::semantic_analyzer::entry::entry(const std::vector<output::jvm_target::access_flag> &access_flags) :
    access_flags(access_flags)
{
}

codesh::semantic_analyzer::entry::~entry() = default;

bool codesh::semantic_analyzer::entry::operator==(const entry &other) const
{
    return get_full_descriptor() == other.get_full_descriptor();
}

const std::vector<codesh::output::jvm_target::access_flag> &codesh::semantic_analyzer::entry::get_access_flags() const
{
    return access_flags;
}

std::string codesh::semantic_analyzer::type_entry::get_full_descriptor() const
{
    return descriptor;
}

const std::list<std::unique_ptr<codesh::semantic_analyzer::entry>> &codesh::semantic_analyzer::type_entry::
    get_entries() const
{
    return entries;
}

std::list<std::unique_ptr<codesh::semantic_analyzer::entry>> &codesh::semantic_analyzer::type_entry::get_entries()
{
    return entries;
}

codesh::semantic_analyzer::variable_entry::variable_entry(const std::vector<output::jvm_target::access_flag> &access_flags,
        std::string type_descriptor) :
    entry(access_flags),
    type_descriptor(std::move(type_descriptor))
{
}

std::string codesh::semantic_analyzer::variable_entry::get_full_descriptor() const
{
    return type_descriptor;
}

codesh::semantic_analyzer::method_entry::method_entry(const std::vector<output::jvm_target::access_flag> &access_flags,
        std::vector<std::string> parameter_descriptors, std::string return_type_descriptor) :
    entry(access_flags),
    parameter_descriptors(std::move(parameter_descriptors)),
    return_type_descriptor(std::move(return_type_descriptor))
{
}

std::string codesh::semantic_analyzer::method_entry::get_full_descriptor() const
{
    std::ostringstream builder;

    builder << '(';

    for (const std::string &param_descriptor : get_parameter_descriptors())
    {
        builder << param_descriptor;
    }

    builder << ')';
    builder << return_type_descriptor;

    return builder.str();
}

const std::vector<std::string> &codesh::semantic_analyzer::method_entry::get_parameter_descriptors() const
{
    return parameter_descriptors;
}
std::string codesh::semantic_analyzer::method_entry::get_return_type_descriptor() const
{
    return return_type_descriptor;
}

const std::stack<std::unique_ptr<codesh::semantic_analyzer::scope>> &codesh::semantic_analyzer::method_entry::get_scopes()
    const
{
    return scopes;
}

std::stack<std::unique_ptr<codesh::semantic_analyzer::scope>> &codesh::semantic_analyzer::method_entry::get_scopes()
{
    return scopes;
}
