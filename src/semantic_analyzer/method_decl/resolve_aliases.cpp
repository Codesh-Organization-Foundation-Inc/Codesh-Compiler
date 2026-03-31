#include "resolve_aliases.h"

#include "parser/ast/method/method_declaration_ast_node.h"
#include "semantic_analyzer/semantic_context.h"
#include "semantic_analyzer/util.h"

/**
 * @returns Whether this type contains a Bereshit method
 */
static bool handle_bereshit_aliases(const codesh::semantic_analyzer::semantic_context &context,
        codesh::semantic_analyzer::type_symbol &type);
static void rename_method(codesh::semantic_analyzer::type_symbol &type,
                          const codesh::ast::method::method_declaration_ast_node &method_node,
                          const std::string &new_name);


void codesh::semantic_analyzer::method_declaration::resolve_aliases(const semantic_context &context, type_symbol &type)
{
    handle_bereshit_aliases(context, type);
}

static bool handle_bereshit_aliases(const codesh::semantic_analyzer::semantic_context &,
        codesh::semantic_analyzer::type_symbol &type)
{
    const auto bereshit = type.get_scope().resolve_local("בראשית");
    if (!bereshit)
        return false;

    const auto bereshit_method_overloads = dynamic_cast<codesh::semantic_analyzer::method_overloads_symbol *>(
        &bereshit.value().get()
    );
    if (!bereshit_method_overloads)
        return false;

    const auto bereshit_method =
        static_cast<codesh::semantic_analyzer::method_overloads_symbol *>(&bereshit.value().get()) // NOLINT(*-pro-type-static-cast-downcast)
            ->resolve_method("[Ljava/lang/String;");

    if (!bereshit_method.has_value())
        return false;

    // From this point, the method has the exact signature of Bereshit.
    // Method name + parameter types.
    // Therefore, it MUST be Bereshit.
    const auto &method_node = *bereshit_method->get().get_producing_node();

    // Validate flags
    const auto &attributes = bereshit_method->get().get_attributes();

    if (attributes.get_visibility() != codesh::definition::visibility::PUBLIC)
        return true;
    if (!attributes.get_is_static())
        return true;

    rename_method(type, method_node, "main");
    return true;
}


static void rename_method(codesh::semantic_analyzer::type_symbol &type,
                          const codesh::ast::method::method_declaration_ast_node &method_node,
                          const std::string &new_name)
{
    const std::string old_name = method_node.get_last_name(false);

    // Get the original method names' overloads
    codesh::semantic_analyzer::method_overloads_symbol &source_method_overloads =
        *static_cast<codesh::semantic_analyzer::method_overloads_symbol *>( // NOLINT(*-pro-type-static-cast-downcast)
            &type.get_scope().resolve_local(old_name)->get()
        );

    // And the new one
    codesh::semantic_analyzer::method_overloads_symbol &dest_method_overloads =
        codesh::semantic_analyzer::util::get_method_overloads_symbol(new_name, type);


    // Move the method symbol from the old overloads table to the new one
    auto &method = dest_method_overloads.get_scope().add_symbol(
        method_node.generate_parameters_descriptor(),
        source_method_overloads.get_scope().resolve_and_move(method_node.generate_parameters_descriptor())
    ).first.get();


    // Perform the actual renaming
    static_cast<codesh::semantic_analyzer::method_symbol *>(&method)->set_full_name( // NOLINT(*-pro-type-static-cast-downcast)
        type.get_full_name().with(new_name)
    );


    // Clean method overloads if no more overloads exist
    const bool is_method_overloads_empty =
        static_cast<const codesh::semantic_analyzer::method_overloads_symbol &>(source_method_overloads)
            .get_scope()
            .is_empty();

    if (is_method_overloads_empty)
    {
        type.get_scope().remove_symbol(old_name);
    }
}
