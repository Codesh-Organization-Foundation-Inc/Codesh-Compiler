#include "scope.h"

codesh::semantic_analyzer::scope::scope() = default;
codesh::semantic_analyzer::scope::scope(scope *const parent_scope) : parent_scope(*parent_scope)
{}

const std::unordered_map<std::string, std::unique_ptr<codesh::semantic_analyzer::entry>> &codesh::semantic_analyzer::scope::
    get_entries() const
{
    return entries;
}

std::unordered_map<std::string, std::unique_ptr<codesh::semantic_analyzer::entry>> codesh::semantic_analyzer::scope::
    get_entries()
{
    return entries;
}

const std::optional<std::reference_wrapper<codesh::semantic_analyzer::scope>> &codesh::semantic_analyzer::scope::
    get_parent_scope() const
{
    return parent_scope;
}
