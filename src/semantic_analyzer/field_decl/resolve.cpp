#include "resolve.h"

#include "parser/ast/type_declaration/field_declaration_ast_node.h"
#include "semantic_analyzer/semantic_context.h"
#include "semantic_analyzer/symbol_table/symbol.h"
#include "semantic_analyzer/util.h"

void codesh::semantic_analyzer::field_declaration::resolve(const semantic_context &context,
        const type_symbol &type, const ast::type_decl::field_declaration_ast_node &field_decl)
{
    const auto field_sym_opt = type.get_scope().resolve_local(field_decl.get_name());
    if (!field_sym_opt.has_value())
        return;

    const auto &field_sym = *static_cast<field_symbol *>(
        &field_sym_opt->get()
    );

    util::resolve_type_node(
        context,
        *field_decl.get_type(),
        *field_sym.get_type()
    );
}
