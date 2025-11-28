#include "method_declaration.h"

#include "../../parser/ast/compilation_unit_ast_node.h"
#include "../../parser/ast/type/custom_type_ast_node.h"
#include "../../parser/ast/type/primitive_type_ast_node.h"
#include "../../parser/ast/type_declaration/class_declaration_ast_node.h"
#include "../errors/errors.h"
#include "util.h"

[[nodiscard]] static codesh::semantic_analyzer::method_overloads_symbol &get_method_overloads(const std::string &name,
        codesh::semantic_analyzer::type_symbol &containing_type);

static std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> clone_parameter_types(
        const codesh::ast::method_declaration_ast_node &method_decl);

static void resolve_return_type(
    const codesh::ast::compilation_unit_ast_node &root,
    const codesh::ast::method_declaration_ast_node &method_decl,
    const std::string &class_name
);

static void resolve_parameters(
    const codesh::ast::compilation_unit_ast_node &root,
    const codesh::ast::method_declaration_ast_node &method,
    const std::string &class_name
);

static void handle_bereshit_aliases(codesh::semantic_analyzer::type_symbol &type);


void codesh::semantic_analyzer::method_declaration::collect_methods(const ast::type_decl::class_declaration_ast_node &class_decl,
                                                type_symbol &containing_type)
{
    for (const auto &method_decl : class_decl.get_methods())
    {
        method_overloads_symbol &methods_container = get_method_overloads(method_decl->get_name(), containing_type);

        const auto [it, inserted] = methods_container.add_symbol(
            method_decl->generate_parameters_descriptor(false), std::make_unique<method_symbol>(
                method_decl->get_attributes()->get_access_flags(),
                clone_parameter_types(*method_decl),
                method_decl->get_return_type()->clone(),
                *method_decl
            )
        );

        if (!inserted)
        {
            //TODO: Print full method declaration
            std::ostringstream os_string;
            os_string << "Duplicate method declared: " << method_decl->get_name();

            collect_error(os_string.str());
        }

        //TODO: Collect local variables
    }
}

static codesh::semantic_analyzer::method_overloads_symbol &get_method_overloads(const std::string &name,
        codesh::semantic_analyzer::type_symbol &containing_type)
{
    return containing_type.add_symbol(
        name, std::make_unique<codesh::semantic_analyzer::method_overloads_symbol>()
    ).first;
}

static std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> clone_parameter_types(
        const codesh::ast::method_declaration_ast_node &method_decl)
{
    std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> result;

    for (const auto &param_node : method_decl.get_parameters())
    {
        result.push_back(param_node->get_type()->clone());
    }

    return result;
}


void codesh::semantic_analyzer::method_declaration::resolve_methods(const ast::compilation_unit_ast_node &root) {
    //TODO: Properly get country of origin
    const country_symbol &country = root.get_symbol_table()->get().resolve_country("").value();

    for (const auto &type_decl : root.get_type_declarations())
    {
        auto *class_node = dynamic_cast<ast::type_decl::class_declaration_ast_node *>(type_decl.get());
        if (!class_node)
            continue;

        // Get type symbol
        const type_symbol &type = *static_cast<type_symbol *>(&country.resolve(type_decl->get_name()).value().get()); // NOLINT(*-pro-type-static-cast-downcast)

        for (const auto &method_decl : class_node->get_methods())
        {
            method_overloads_symbol &method_overloads = *static_cast<method_overloads_symbol *>( // NOLINT(*-pro-type-static-cast-downcast)
                &type.resolve(method_decl->get_name()).value().get()
            );

            std::unique_ptr<method_symbol> method(
                static_cast<method_symbol *>( // NOLINT(*-pro-type-static-cast-downcast)
                    method_overloads.resolve_and_move(method_decl->generate_parameters_descriptor(false))
                        .release()
                )
            );

            resolve_return_type(root, *method_decl, class_node->get_name());
            resolve_parameters(root, *method_decl, class_node->get_name());


            // Move to a new overloads entry now that the parameters' descriptors are real
            method_overloads.add_symbol(method_decl->generate_parameters_descriptor(), std::move(method));
        }

    }
}


static void resolve_return_type(
    const codesh::ast::compilation_unit_ast_node &root,
    const codesh::ast::method_declaration_ast_node &method_decl,
    const std::string &class_name
) {
    auto *return_type = dynamic_cast<codesh::ast::type::custom_type_ast_node *>(method_decl.get_return_type());

    if (!return_type)
    {
        // Primitive types bound to be okay
        return;
    }


    //TODO: Use actual countries
    const std::vector lookup_countries = {
        root.get_symbol_table().value().get().resolve_country("").value()
    };


    if (!codesh::semantic_analyzer::util::resolve_custom_type_node(lookup_countries, *return_type))
    {
        std::ostringstream os_string;
        os_string << "Unknown return type " << return_type->get_name()
            << " in method " << method_decl.get_name()
            << " of type " << class_name;

        codesh::semantic_analyzer::collect_error(os_string.str());
    }
}

static void resolve_parameters(
        const codesh::ast::compilation_unit_ast_node &root,
        const codesh::ast::method_declaration_ast_node &method,
        const std::string &class_name)
{
    for (const auto &param : method.get_parameters())
    {
        auto *custom_param = dynamic_cast<codesh::ast::type::custom_type_ast_node *>(param->get_type());

        if (!custom_param)
            continue;


        //TODO: Use actual countries
        const std::vector lookup_countries = {
            root.get_symbol_table().value().get().resolve_country("").value()
        };


        if (!codesh::semantic_analyzer::util::resolve_custom_type_node(lookup_countries, *custom_param))
        {
            std::ostringstream os_string;
            os_string << "Unknown return type " << custom_param->get_name()
                << " in method " << method.get_name()
                << " of type " << class_name;

            codesh::semantic_analyzer::collect_error(os_string.str());
        }
    }
}


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


    codesh::ast::method_declaration_ast_node &method_node = bereshit_method->get().get_producing_node();


    // Validate flags
    bool is_public = false;
    bool is_static = false;

    for (auto &method_access_flag : bereshit_method->get().get_access_flags())
    {
        if (method_access_flag == codesh::output::jvm_target::access_flag::ACC_PUBLIC)
            is_public = true;
        if (method_access_flag == codesh::output::jvm_target::access_flag::ACC_STATIC)
            is_static = true;
    }

    if (!(is_public && is_static))
    {
        codesh::semantic_analyzer::collect_error(
            "The method 'בראשית' must be both public and static."
        );
        return;
    }


    // Perform renaming!!
    auto &new_method_overloads = get_method_overloads("main", type);

    new_method_overloads.add_symbol(
        method_node.generate_parameters_descriptor(),
        bereshit_method_overloads->resolve_and_move(method_node.generate_parameters_descriptor())
    );

    method_node.set_name("main");


    // Clean method overloads if no more overloads exist
    const bool is_method_overloads_empty =
        static_cast<const codesh::semantic_analyzer::method_overloads_symbol &>(*bereshit_method_overloads)
            .get_symbol_map()
            .empty();

    if (is_method_overloads_empty)
    {
        type.remove_symbol("בראשית");
    }
}
