#pragma once
#include "../../parser/ast/method/method_declaration_ast_node.h"

namespace codesh::semantic_analyzer
{
class country_symbol;
class method_symbol;
}

namespace codesh::semantic_analyzer::method_call
{
void resolve(const std::vector<std::reference_wrapper<country_symbol>> &lookup_countries,
        const ast::method::method_declaration_ast_node &method_decl, method_symbol &method);
}
