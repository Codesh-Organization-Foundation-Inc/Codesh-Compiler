#pragma once

namespace codesh::semantic_analyzer
{
struct semantic_context;
}
namespace codesh::ast::method
{
class method_declaration_ast_node;
}
namespace codesh::semantic_analyzer
{
class type_symbol;
}
namespace codesh::ast
{
class compilation_unit_ast_node;
}


namespace codesh::semantic_analyzer::method_declaration
{
void resolve(const semantic_context &context, const type_symbol &type,
        const ast::method::method_declaration_ast_node &method_decl);
}