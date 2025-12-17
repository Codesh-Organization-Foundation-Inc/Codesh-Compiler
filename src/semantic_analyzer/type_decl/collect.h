#pragma once

#include "../../blasphemy/blasphemy_consumer.h"
#include "../analyzer.h"

namespace codesh::semantic_analyzer
{
class country_symbol;
}
namespace codesh::ast
{
class compilation_unit_ast_node;
}


namespace codesh::semantic_analyzer::type_declaration
{
void collect_types(const ast::compilation_unit_ast_node &root_node, country_symbol &containing_country,
        const blasphemy::blasphemy_consumer &blasphemy_consumer);
}
