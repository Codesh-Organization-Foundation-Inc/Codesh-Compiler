#include "resolve_aliases.h"
#include "../method_decl/resolve_aliases.h"

#include "../../parser/ast/compilation_unit_ast_node.h"
#include "../semantic_context.h"

void codesh::semantic_analyzer::type_declaration::resolve_aliases(const semantic_context &context, type_symbol &type,
        const ast::type_decl::type_declaration_ast_node &type_decl)
{
    const std::string name = type_decl.get_name();
    const semantic_context new_context = context.with_consumer("בָּעֶצֶם", name);

    method_declaration::resolve_aliases(new_context, type);
}
