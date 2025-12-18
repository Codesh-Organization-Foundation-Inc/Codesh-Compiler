#include "semantic_context.h"

codesh::semantic_analyzer::semantic_context::semantic_context(const ast::compilation_unit_ast_node &root,
                                                              const blasphemy::blasphemy_consumer &blasphemy_consumer) :
    root(root),
    blasphemy_consumer(blasphemy_consumer)
{
}

codesh::semantic_analyzer::semantic_context codesh::semantic_analyzer::semantic_context::with_consumer(
    const blasphemy::blasphemy_consumer &blasphemy_consumer) const
{
    return semantic_context(root, blasphemy_consumer);
}
