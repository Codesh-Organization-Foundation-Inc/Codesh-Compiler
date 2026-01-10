#pragma once

#include "blasphemy/blasphemy_consumer.h"

namespace codesh::ast
{
class compilation_unit_ast_node;
}


namespace codesh::semantic_analyzer
{
class country_symbol;

struct semantic_context
{
    semantic_context(const std::vector<std::reference_wrapper<country_symbol>> &lookup_countries,
            const ast::compilation_unit_ast_node &root,
            blasphemy::blasphemy_consumer blasphemy_consumer);

    const std::vector<std::reference_wrapper<country_symbol>> &lookup_countries;
    const ast::compilation_unit_ast_node &root;
    const blasphemy::blasphemy_consumer blasphemy_consumer;

    [[nodiscard]] semantic_context with_consumer(blasphemy::blasphemy_consumer blasphemy_consumer) const;

    /**
     * Constructs a new inner semantic context.
     * Thrown blasphemies will have their wrapper names appended.
     */
    [[nodiscard]] semantic_context with_consumer(const std::string &wrapper_name, const std::string &object_name) const;
};

}
