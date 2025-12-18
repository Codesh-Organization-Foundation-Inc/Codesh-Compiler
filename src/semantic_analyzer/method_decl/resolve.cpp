#include "resolve.h"

#include "../../blasphemy/blasphemy_collector.h"
#include "../../parser/ast/compilation_unit_ast_node.h"
#include "../../parser/ast/type/custom_type_ast_node.h"
#include "../../parser/ast/type/primitive_type_ast_node.h"
#include "../util.h"
#include "../semantic_context.h"

#include <functional>
#include <ranges>

static void resolve_return_type(const codesh::semantic_analyzer::semantic_context &context,
        const codesh::ast::method::method_declaration_ast_node &method_decl,
        const codesh::semantic_analyzer::method_symbol &method_symbol);

static void resolve_parameters(const codesh::semantic_analyzer::semantic_context &context,
        const codesh::ast::method::method_declaration_ast_node &method,
        const codesh::semantic_analyzer::method_symbol &method_symbol);

static void resolve_local_variables(const codesh::semantic_analyzer::semantic_context &context,
                                    const codesh::semantic_analyzer::method_symbol &method_symbol);

static void resolve_method_signature(const codesh::semantic_analyzer::semantic_context &context,
                                     const codesh::ast::method::method_declaration_ast_node &method_decl,
                                     const codesh::semantic_analyzer::type_symbol &type);


void codesh::semantic_analyzer::method_declaration::resolve(
    const semantic_context &context, const type_symbol &type,
    const ast::method::method_declaration_ast_node &method_decl)
{
    resolve_method_signature(context, method_decl, type);
}

static void resolve_method_signature(const codesh::semantic_analyzer::semantic_context &context,
                                     const codesh::ast::method::method_declaration_ast_node &method_decl,
                                     const codesh::semantic_analyzer::type_symbol &type)
{
    const std::string name = method_decl.get_name();
    const auto new_context = context.with_consumer("בְּמַעֲשֶׂה", name);

    auto &method_overloads = *static_cast<codesh::semantic_analyzer::method_overloads_symbol *>( // NOLINT(*-pro-type-static-cast-downcast)
        &type.resolve(name).value().get()
    );


    // Get relevant method symbol from the method overloads map
    // Then cast it to method_symbol
    std::unique_ptr<codesh::semantic_analyzer::method_symbol> method(
        static_cast<codesh::semantic_analyzer::method_symbol *>( // NOLINT(*-pro-type-static-cast-downcast)
            method_overloads.resolve_and_move(method_decl.generate_parameters_descriptor(false))
                .release()
        )
    );

    resolve_return_type(new_context, method_decl, *method);
    resolve_parameters(new_context, method_decl, *method);
    resolve_local_variables(new_context, *method);

    // Move to a new overloads entry, now that the parameters' descriptors are real
    method_overloads.add_symbol(method_decl.generate_parameters_descriptor(), std::move(method));
}

static void resolve_return_type(const codesh::semantic_analyzer::semantic_context &context,
        const codesh::ast::method::method_declaration_ast_node &method_decl,
        const codesh::semantic_analyzer::method_symbol &method_symbol)
{
    auto *return_type = dynamic_cast<codesh::ast::type::custom_type_ast_node *>(method_decl.get_return_type());

    if (!return_type)
    {
        // Primitive types bound to be okay
        return;
    }

    if (!codesh::semantic_analyzer::util::resolve_custom_type_node(
        context.lookup_countries,
        *return_type,
        method_symbol.get_return_type()
    )) {
        context.blasphemy_consumer(fmt::format(
            "עֶצֶם בִּלְתִּי מְזֹהֶה: סוּג הֶחְזֵר לֹא יָדוּעַ {}",
            return_type->get_name()
        ));
    }
}

static void resolve_parameters(const codesh::semantic_analyzer::semantic_context &context,
        const codesh::ast::method::method_declaration_ast_node &method,
        const codesh::semantic_analyzer::method_symbol &method_symbol)
{
    size_t i = 0;
    for (const auto &param : method.get_parameters())
    {
        auto *custom_param = dynamic_cast<codesh::ast::type::custom_type_ast_node *>(param->get_type());
        if (!custom_param)
            continue;

        if (!codesh::semantic_analyzer::util::resolve_custom_type_node(
            context.lookup_countries,
            *custom_param,
            *method_symbol.get_parameter_types()[i]
        )) {
            context.blasphemy_consumer(fmt::format(
                "עֶצֶם בִּלְתִּי מְזֹהֶה: סוּג מִנְחָה לֹא יְדוּעָה {}",
                custom_param->get_name()
            ));
        }

        ++i;
    }
}

static void resolve_local_variables(const codesh::semantic_analyzer::semantic_context &context,
                                    const codesh::semantic_analyzer::method_symbol &method_symbol)
{
    for (const auto &var_symbol : method_symbol.get_scope().get_variables() | std::views::values)
    {
        auto *custom_param = dynamic_cast<codesh::ast::type::custom_type_ast_node *>(var_symbol->get_type());
        if (!custom_param)
            continue;

        if (!codesh::semantic_analyzer::util::resolve_custom_type_node(
            context.lookup_countries,
            *custom_param
        )) {
            context.blasphemy_consumer(fmt::format(
                "עֶצֶם בִּלְתִּי מְזֹהֶה: סוּג לֹא יָדוּעַ {}",
                custom_param->get_name()
            ));
        }
    }
}
