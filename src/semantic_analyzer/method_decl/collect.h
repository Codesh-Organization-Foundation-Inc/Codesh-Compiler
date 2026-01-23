#pragma once
#include "parser/ast/method/method_declaration_ast_node.h"

namespace codesh::ast::type_decl
{
class type_declaration_ast_node;
}
namespace codesh::semantic_analyzer
{
struct semantic_context;
}
namespace codesh::semantic_analyzer
{
class type_symbol;
}


namespace codesh::semantic_analyzer::method_declaration
{
void collect(const semantic_context &context, ast::method::method_declaration_ast_node &method_decl,
             type_symbol &containing_type);
}
