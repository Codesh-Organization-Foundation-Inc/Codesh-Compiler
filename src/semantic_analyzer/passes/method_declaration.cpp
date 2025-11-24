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
        auto *custom_param = dynamic_cast<codesh::ast::type::custom_type_ast_node *>(param.get());

        if (!custom_param)
        {
            // Primitive types bound to be okay
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
    const std::string orig_name = "בראשית";
    const std::string new_name = "Main";

    const std::string expected_descriptor = method_decl.generate_descriptor(true);

    // check if name is בראשית
    if (method_decl.get_name() != orig_name)
        return;

    // check if parameter type is כתובים כמסדר
    if (expected_descriptor != "([Ljava/lang/String;)V")
        return;

    const codesh::semantic_analyzer::named_scope_map &type_map = static_cast<const codesh::semantic_analyzer::i_scope_containing_symbol &>(type).get_symbol_map();
    auto it_overloads = type_map.find(orig_name);
    if (it_overloads == type_map.end())
    {
        std::ostringstream os;
        os << "Internal: expected overloads for '" << orig_name << "' in type but not found.";
        codesh::semantic_analyzer::collect_error(os.str());
        return;
    }

    // The overloads container must be a methods_overloads_symbol
    auto *orig_overloads = dynamic_cast<codesh::semantic_analyzer::methods_overloads_symbol *>(it_overloads->second.get());
    if (!orig_overloads) {
        std::ostringstream os;
        os << "Internal: symbol for '" << orig_name << "' is not a methods_overloads_symbol.";
        codesh::semantic_analyzer::collect_error(os.str());
        return;
    }


}
