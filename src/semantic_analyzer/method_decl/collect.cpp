#include "collect.h"

#include "../../parser/ast/compilation_unit_ast_node.h"
#include "../../parser/ast/type/primitive_type_ast_node.h"
#include "../../parser/ast/type_declaration/class_declaration_ast_node.h"
#include "../errors/errors.h"
#include "../util.h"

static std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> clone_parameter_types(
        const codesh::ast::method::method_declaration_ast_node &method_decl);


void codesh::semantic_analyzer::method_declaration::collect_methods(const ast::type_decl::class_declaration_ast_node &class_decl,
                                                type_symbol &containing_type)
{
    for (const auto &method_decl : class_decl.get_all_methods())
    {
        method_overloads_symbol &methods_container =
            util::get_method_overloads_symbol(method_decl->get_name(), containing_type);

        const auto [it, inserted] = methods_container.add_symbol(
            method_decl->generate_parameters_descriptor(false), std::make_unique<method_symbol>(
                method_decl->get_attributes()->get_access_flags(),
                clone_parameter_types(*method_decl),
                method_decl->get_return_type()->clone(),
                method_decl.get()
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

static std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> clone_parameter_types(
        const codesh::ast::method::method_declaration_ast_node &method_decl)
{
    std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> result;

    for (const auto &param_node : method_decl.get_parameters())
    {
        result.push_back(param_node->get_type()->clone());
    }

    return result;
}

