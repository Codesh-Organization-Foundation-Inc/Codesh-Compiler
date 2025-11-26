#include "method_declaration.h"

#include "../../parser/ast/compilation_unit_ast_node.h"
#include "../../parser/ast/type/custom_type_ast_node.h"
#include "../../parser/ast/type/primitive_type_ast_node.h"
#include "../../parser/ast/type_declaration/class_declaration_ast_node.h"
#include "../errors/errors.h"
#include "util.h"

static codesh::semantic_analyzer::methods_overloads_symbol &get_method_overloads(
        const codesh::ast::method_declaration_ast_node &method_decl,
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

static void resolve_main_method(
    codesh::semantic_analyzer::type_symbol &type,
    const codesh::ast::method_declaration_ast_node &method_decl
);


void codesh::semantic_analyzer::method_declaration::collect_methods(const ast::type_decl::class_declaration_ast_node &class_decl,
                                                type_symbol &containing_type)
{
    for (const auto &method_decl : class_decl.get_methods())
    {
        methods_overloads_symbol &methods_container = get_method_overloads(*method_decl, containing_type);

        const auto [it, inserted] = methods_container.add_symbol(
            method_decl->generate_parameter_descriptors(false), std::make_unique<method_symbol>(
                method_decl->get_attributes()->get_access_flags(),
                clone_parameter_types(*method_decl),
                method_decl->get_return_type()->clone()
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

static codesh::semantic_analyzer::methods_overloads_symbol &get_method_overloads(
        const codesh::ast::method_declaration_ast_node &method_decl,
        codesh::semantic_analyzer::type_symbol &containing_type)
{
    return containing_type.add_symbol(
        method_decl.get_name(), std::make_unique<codesh::semantic_analyzer::methods_overloads_symbol>()
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
            const method_symbol &method = *static_cast<method_symbol *>(&type.resolve(method_decl->get_name()).value().get()); // NOLINT(*-pro-type-static-cast-downcast)

            resolve_return_type(root, *method_decl, class_node->get_name());
            resolve_parameters(root, *method_decl, class_node->get_name());

            //TODO: Update method entry in the method scope table
            //method.
        }

        auto &mutable_type = const_cast<type_symbol &>(type);
        for (const auto &method_decl_ptr : class_node->get_methods())
        {
            resolve_main_method(mutable_type, *method_decl_ptr);
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
    const std::string &class_name
) {
    for (const auto &param : method.get_parameters())
    {
        auto *custom_param = dynamic_cast<codesh::ast::type::custom_type_ast_node *>(param->get_type());

        if (!custom_param)
        {
            continue;
        }


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

static void resolve_main_method(
    codesh::semantic_analyzer::type_symbol &type,
    const codesh::ast::method_declaration_ast_node &method_decl
) {
    const std::string original_name = "בראשית";
    const std::string new_name  = "Main"; // change to main if needed

    // Only match בראשית
    if (method_decl.get_name() != original_name)
        return;


    const std::string full_descriptor = method_decl.generate_descriptor(true);
    const std::string descriptor_key  = method_decl.generate_parameter_descriptors(true);

    // check signatures (returns void and takes String[])
    if (full_descriptor != "([Ljava/lang/String;)V")
        return;

    // Resolve the overload
    const auto overload_ref = type.resolve(original_name);
    if (!overload_ref)
        return;

    const auto *overloads =
        dynamic_cast<codesh::semantic_analyzer::methods_overloads_symbol*>(&overload_ref->get());
    if (!overloads)
        return;

    // Resolve the specific method by descriptor
    const auto method_ref = overloads->resolve(descriptor_key);
    if (!method_ref)
        return;

    const auto *method_sym =
        dynamic_cast<codesh::semantic_analyzer::method_symbol*>(&method_ref->get());
    if (!method_sym)
        return;

    // Validate flags
    {
        bool is_public = false, is_static = false;
        for (auto &method_access_flag : method_sym->get_access_flags()) {
            if (method_access_flag == codesh::output::jvm_target::access_flag::ACC_PUBLIC) is_public = true;
            if (method_access_flag == codesh::output::jvm_target::access_flag::ACC_STATIC) is_static = true;
        }
        if (!is_public || !is_static)
        {
            codesh::semantic_analyzer::collect_error(
                "The method 'בראשית' must be public and static."
            );
            return;
        }
    }

    // Clone param types
    std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> cloned_params;
    for (auto &p : method_sym->get_parameter_types())
        cloned_params.push_back(p->clone());

    // Clone return type
    auto cloned_return = method_sym->get_return_type().clone();

    auto new_method = std::make_unique<codesh::semantic_analyzer::method_symbol>(
        method_sym->get_access_flags(),
        std::move(cloned_params),
        std::move(cloned_return)
    );

    // Ensure an overload container exists for "Main"
    const auto pair_res =
        type.add_symbol(new_name, std::make_unique<codesh::semantic_analyzer::methods_overloads_symbol>());

    auto *main_overloads = &pair_res.first.get();
    if (!main_overloads)
    {
        codesh::semantic_analyzer::collect_error(
            "Internal: Failed to access Main overload container.");
        return;
    }

    // Add new method into "Main" overloads
    auto [_, inserted] = main_overloads->add_symbol(descriptor_key, std::move(new_method));
    if (!inserted)
    {
        codesh::semantic_analyzer::collect_error(
            "Duplicate Main(String[]) found.");
        return;
    }

    // Set ast name
    const_cast<codesh::ast::method_declaration_ast_node&>(method_decl).set_name(new_name);
}

