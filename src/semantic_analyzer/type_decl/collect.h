#pragma once

#include "../analyzer.h"

namespace codesh::semantic_analyzer
{
struct semantic_context;
}
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
void collect(const semantic_context &context, ast::type_decl::type_declaration_ast_node &type_decl,
        country_symbol &country);
}
