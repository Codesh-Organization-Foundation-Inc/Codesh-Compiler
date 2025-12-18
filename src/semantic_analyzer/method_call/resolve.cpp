#include "resolve.h"

#include "../../parser/ast/method/operation/method_call_ast_node.h"
#include "../semantic_context.h"
#include "../util.h"
#include "fmt/color.h"

#include <ranges>

static std::optional<std::reference_wrapper<codesh::semantic_analyzer::method_symbol>> resolve_method_call(
        const codesh::semantic_analyzer::semantic_context &context,
        const codesh::semantic_analyzer::method_symbol &containing_method,
        codesh::ast::method::operation::method_call_ast_node &method_call);

static std::optional<std::reference_wrapper<codesh::semantic_analyzer::method_symbol>> get_called_method_as_symbol(
        const codesh::semantic_analyzer::semantic_context &context,
        const codesh::semantic_analyzer::type_symbol &type,
        const codesh::ast::method::operation::method_call_ast_node &method_call);

static bool are_types_compatible(const codesh::ast::type::type_ast_node &from,
        const codesh::ast::type::type_ast_node &to);


void codesh::semantic_analyzer::method_call::resolve(const semantic_context &context,
                                                     ast::method::operation::method_call_ast_node &method_call,
                                                     const method_symbol &containing_method)
{
    const auto result = resolve_method_call(
        context,
        containing_method,
        method_call
    );

    if (!result.has_value())
        return;

    //TODO: Resolve aliases
}


static std::optional<std::reference_wrapper<codesh::semantic_analyzer::method_symbol>> resolve_method_call(
        const codesh::semantic_analyzer::semantic_context &context,
        const codesh::semantic_analyzer::method_symbol &containing_method,
        codesh::ast::method::operation::method_call_ast_node &method_call)
{
    const auto &name = method_call.get_name();

    if (!name.is_single_part())
    {
        //TODO: If it is found the the name is an FQCN itself, start the search from the top of the symbol table.
        throw std::runtime_error("Non-single FQCNs not yet supported");
    }

    // Since this is a single-part FQCN (name only), the method must either be the classes' or a static import.

    // THIS:
    const auto &parent_type = containing_method.get_parent_type();
    const auto method = get_called_method_as_symbol(
        context,
        parent_type,
        method_call
    );

    //TODO: Handle static imports

    if (!method.has_value())
        return std::nullopt;

    // Update the AST node to the found result
    method_call.set_resolved_name(
        parent_type.get_full_name().with(name.get_last_part())
    );

    return method;
}

static std::optional<std::reference_wrapper<codesh::semantic_analyzer::method_symbol>> get_called_method_as_symbol(
        const codesh::semantic_analyzer::semantic_context &context,
        const codesh::semantic_analyzer::type_symbol &type,
        const codesh::ast::method::operation::method_call_ast_node &method_call)
{
    const auto method_overloads_raw = type.resolve(method_call.get_name().get_last_part());
    if (!method_overloads_raw)
    {
        //TODO: Throw "name doesn't exist"
        context.blasphemy_consumer(fmt::format(
            "היי אלירןןןןןן תעשה את השם בבקשה השגיאה היא שהדבר לא נמצא"
        ));
        return std::nullopt;
    }

    const auto *method_overloads = dynamic_cast<const codesh::semantic_analyzer::method_overloads_symbol *>(&method_overloads_raw->get());
    if (!method_overloads)
    {
        //TODO: Throw "is not a method"
        context.blasphemy_consumer(fmt::format(
            "אליצ'אאאןןן >w< איייי!! נאני גה־סוקי? זוהי לא מתודה, יורימו אנאטה?"
        ));
        return std::nullopt;
    }


    // Verify parameter types

    for (const auto &symbol : method_overloads->get_symbol_map() | std::views::values)
    {
        auto &method = *static_cast<codesh::semantic_analyzer::method_symbol *>(symbol.get()); // NOLINT(*-pro-type-static-cast-downcast)

        const auto &method_params = method.get_parameter_types();
        const auto &arguments = method_call.get_arguments();

        if (method_params.size() != arguments.size())
            continue;


        for (size_t i = 0; i < method_params.size(); i++)
        {
            const auto method_param_type = method_params.at(i).get();
            const auto argument_value = method_call.get_arguments().at(i).get();

            if (are_types_compatible(argument_value->get_type(), *method_param_type))
            {
                //TODO: Consider "best match", don't just return (casting etc.)
                return method;
            }
        }
    }

    //TODO: Throw "mismatched argument types"
    context.blasphemy_consumer(fmt::format(
        "מאסתי"
    ));
    return std::nullopt;
}

static bool are_types_compatible(const codesh::ast::type::type_ast_node &from,
        const codesh::ast::type::type_ast_node &to)
{
    //FIXME: Should check for auto conversions if not an exact match.
    return from.generate_descriptor() == to.generate_descriptor();
}
