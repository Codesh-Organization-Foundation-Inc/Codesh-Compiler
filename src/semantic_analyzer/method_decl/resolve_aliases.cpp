#include "resolve_aliases.h"

#include "../../parser/ast/compilation_unit_ast_node.h"
#include "../util.h"

static void handle_bereshit_aliases(codesh::semantic_analyzer::type_symbol &type);
static void rename_method(codesh::semantic_analyzer::type_symbol &type,
                          codesh::ast::method::method_declaration_ast_node &method_node, const std::string &new_name);


void codesh::semantic_analyzer::method_declaration::resolve_aliases(type_symbol &type)
{
    handle_bereshit_aliases(type);
}

static void handle_bereshit_aliases(codesh::semantic_analyzer::type_symbol &type)
{
    const auto bereshit = type.resolve("בראשית");
    if (!bereshit)
        return;

    const auto bereshit_method_overloads = dynamic_cast<codesh::semantic_analyzer::method_overloads_symbol *>(&bereshit.value().get());
    if (!bereshit_method_overloads)
        return;

    const auto bereshit_method =
        static_cast<codesh::semantic_analyzer::method_overloads_symbol *>(&bereshit.value().get()) // NOLINT(*-pro-type-static-cast-downcast)
            ->resolve_method("[Ljava/lang/String;");

    if (!bereshit_method.has_value())
        return;

    codesh::ast::method::method_declaration_ast_node &method_node = *bereshit_method->get().get_producing_node();


    // Validate flags
    bool is_public = false;
    bool is_static = false;

    for (auto &method_access_flag : bereshit_method->get().get_access_flags())
    {
        if (method_access_flag == codesh::output::jvm_target::access_flag::ACC_PUBLIC)
            is_public = true;
        if (method_access_flag == codesh::output::jvm_target::access_flag::ACC_STATIC)
            is_static = true;

        if (is_public && is_static)
            break;
    }

    if (!(is_public && is_static))
        return;


    rename_method(type, method_node, "main");
}


static void rename_method(codesh::semantic_analyzer::type_symbol &type,
                          codesh::ast::method::method_declaration_ast_node &method_node, const std::string &new_name)
{
    // Get the original method names' overloads
    codesh::semantic_analyzer::method_overloads_symbol &source_method_overloads =
        *static_cast<codesh::semantic_analyzer::method_overloads_symbol *>( // NOLINT(*-pro-type-static-cast-downcast)
            &type.resolve(method_node.get_name())->get()
        );

    // And the new one
    codesh::semantic_analyzer::method_overloads_symbol &dest_method_overloads =
        codesh::semantic_analyzer::util::get_method_overloads_symbol(new_name, type);


    // Move the method symbol from the old overloads table to the new one.
    // This will rename it since the `dest_method_overloads` is only accessible via the updated method's name.
    dest_method_overloads.add_symbol(
        method_node.generate_parameters_descriptor(),
        source_method_overloads.resolve_and_move(method_node.generate_parameters_descriptor())
    );

    // Rename the method node as well
    const std::string old_name = method_node.get_name();
    method_node.set_name(new_name);


    // Clean method overloads if no more overloads exist
    const bool is_method_overloads_empty =
        static_cast<const codesh::semantic_analyzer::method_overloads_symbol &>(source_method_overloads)
            .get_symbol_map()
            .empty();

    if (is_method_overloads_empty)
    {
        type.remove_symbol(old_name);
    }
}