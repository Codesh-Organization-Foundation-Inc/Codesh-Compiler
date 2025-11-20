#include "verify_methods.h"
#include "../errors/errors.h"
#include "../../parser/ast/type_declaration/class_declaration_ast_node.h"
#include "../../parser/ast/type/primitive_type_ast_node.h"
#include "../../parser/ast/type/custom_type_ast_node.h"
#include "../../parser/ast/compilation_unit_ast_node.h"
#include <unordered_set>


namespace codesh::semantic_analyzer
{
// TODO: do all in name_name and put reference and pointers after
static bool type_exists(const ast::compilation_unit_ast_node &root, const std::string &name)
{
    for (auto &declaration : root.get_type_declarations())
    {
        const auto *class_declaration = dynamic_cast<ast::type_decl::class_declaration_ast_node*>(declaration.get());
        if (!class_declaration) continue;

        if (class_declaration->get_name() == name)
        {
            return true;
        }
    }
    return false;
}

void check_methods(const ast::compilation_unit_ast_node &root)
{
    for (auto &type_decl : root.get_type_declarations())
    {
        auto* class_node = dynamic_cast<ast::type_decl::class_declaration_ast_node*>(type_decl.get());

        if (!class_node)
            continue;

        std::unordered_set<std::string> method_names;

        for (const auto &method : class_node->get_methods())
        {
            const std::string &name = method->get_name();

            // Duplicate method check
            const auto [it, did_insert] = method_names.emplace(name);
            if (!did_insert)
            {
                throw_error("Duplicate method declared: " + name +
                            " in type " + class_node->get_name());
            }

            // checking return type
            const auto &return_type = method->get_return_type();

            if (auto  *custom = dynamic_cast<ast::type::custom_type_ast_node*>(return_type))
            {
                const std::string &type_name = custom->get_name();

                if (!type_exists(root, type_name))
                {
                    throw_error("Unknown return type " + type_name + " in method " + name +
                        " of type " + class_node->get_name()
                    );
                }
            }

            // check parameters
            for (auto &param : method->get_parameters())
            {
                ast::type::type_ast_node *parameter_type = param->get_type();

                if (dynamic_cast<ast::type::primitive_type_ast_node*>(parameter_type))
                    continue;

                auto  *custom_parameter =
                    dynamic_cast<ast::type::custom_type_ast_node*>(parameter_type);

                if (!custom_parameter)
                {
                    throw_error(
                        "Invalid parameter type in method " + name +
                        " of type " + class_node->get_name()
                    );
                }


                const std::string &parameter_type_name = custom_parameter->get_name();

                if (!type_exists(root, parameter_type_name))
                {
                    throw_error( "Unknown parameter type " + parameter_type_name + " in method " + name +
                        " of type " + class_node->get_name()
                    );
                }
            }
        }
    }
}
}