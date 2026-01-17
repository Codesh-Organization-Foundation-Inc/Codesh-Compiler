#pragma once

namespace codesh::semantic_analyzer
{
class country_symbol;
}
namespace codesh::semantic_analyzer
{
struct semantic_context;
}
namespace codesh::ast
{
namespace type_decl
{
class type_declaration_ast_node;
}
class compilation_unit_ast_node;
}

namespace codesh::semantic_analyzer::type_declaration
{
void resolve(const semantic_context &context, const ast::type_decl::type_declaration_ast_node &type_decl,
        const country_symbol &country);
}
