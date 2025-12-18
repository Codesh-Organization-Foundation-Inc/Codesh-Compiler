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
};

}
