#include "resolve.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "parser/ast/type_declaration/field_declaration_ast_node.h"
#include "semantic_analyzer/statement/resolve.h"
#include "semantic_analyzer/semantic_context.h"
#include "semantic_analyzer/symbol_table/symbol.h"
#include "semantic_analyzer/util.h"


void codesh::semantic_analyzer::field_declaration::resolve(const semantic_context &context,
        const type_symbol &type, const ast::type_decl::field_declaration_ast_node &field_decl)
{
    const auto field_sym_opt = type.get_field_scope().resolve_local(field_decl.get_name());
    if (!field_sym_opt.has_value())
        return;

    const auto &field_sym = *static_cast<field_symbol *>( // NOLINT(*-pro-type-static-cast-downcast)
        &field_sym_opt->get()
    );

    util::resolve_type_node(
        context,
        *field_decl.get_type(),
        *field_sym.get_type()
    );

    if (field_decl.get_value() != nullptr)
    {
        if (!field_sym.get_attributes().get_is_static())
        {
            blasphemy::get_blasphemy_collector().add_blasphemy(
                blasphemy::details::NON_STATIC_FIELD_INITIALIZATION,
                blasphemy::blasphemy_type::SEMANTIC,
                field_decl.get_code_position()
            );
        }
        else
        {
            statement::resolve_constant_expr(context, *field_decl.get_value());
        }
    }
}
