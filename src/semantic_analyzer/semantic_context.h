#pragma once

#include "../blasphemy/blasphemy_consumer.h"

namespace codesh::ast
{
class compilation_unit_ast_node;
}


namespace codesh::semantic_analyzer
{

struct semantic_context
{
    semantic_context(const ast::compilation_unit_ast_node &root,
            const blasphemy::blasphemy_consumer &blasphemy_consumer);

    const ast::compilation_unit_ast_node &root;
    const blasphemy::blasphemy_consumer &blasphemy_consumer;

    semantic_context with_consumer(const blasphemy::blasphemy_consumer &blasphemy_consumer) const;

    /**
     * Constructs a new inner semantic context.
     * Thrown blasphemies will have their wrapper names appended.
     */
    semantic_context with_consumer(std::string wrapper_name, std::string object_name) const;
};

}
