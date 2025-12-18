#include "semantic_context.h"

#include "fmt/xchar.h"

codesh::semantic_analyzer::semantic_context::semantic_context(const std::vector<std::reference_wrapper<country_symbol>> &lookup_countries,
        const ast::compilation_unit_ast_node &root,
        const blasphemy::blasphemy_consumer &blasphemy_consumer) :
    lookup_countries(lookup_countries),
    root(root),
    blasphemy_consumer(blasphemy_consumer)
{
}

codesh::semantic_analyzer::semantic_context codesh::semantic_analyzer::semantic_context::with_consumer(
    const blasphemy::blasphemy_consumer &blasphemy_consumer) const
{
    return {lookup_countries, root, blasphemy_consumer};
}

codesh::semantic_analyzer::semantic_context codesh::semantic_analyzer::semantic_context::with_consumer(
    const std::string &wrapper_name, const std::string &object_name) const
{
    return with_consumer(blasphemy::create_nested_consumer(blasphemy_consumer, wrapper_name, object_name));
}
