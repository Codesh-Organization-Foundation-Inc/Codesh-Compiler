#include "semantic_context.h"

#include "fmt/xchar.h"

codesh::semantic_analyzer::semantic_context::semantic_context(const ast::compilation_unit_ast_node &root,
                                                              const blasphemy::blasphemy_consumer &blasphemy_consumer) :
    root(root),
    blasphemy_consumer(blasphemy_consumer)
{
}

codesh::semantic_analyzer::semantic_context codesh::semantic_analyzer::semantic_context::with_consumer(
    const blasphemy::blasphemy_consumer &blasphemy_consumer) const
{
    return {root, blasphemy_consumer};
}

codesh::semantic_analyzer::semantic_context codesh::semantic_analyzer::semantic_context::with_consumer(
    std::string wrapper_name, std::string object_name) const
{
    return with_consumer([this, &object_name, &wrapper_name](std::string details) {
        this->blasphemy_consumer(fmt::format(
            "{} {} {}",
            std::move(details),
            std::move(wrapper_name),
            std::move(object_name)
        ));
    });
}
