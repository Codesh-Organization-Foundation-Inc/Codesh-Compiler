#include "check_methods.h"

#include "../../parser/ast/compilation_unit_ast_node.h"
#include "../../parser/ast/type/custom_type_ast_node.h"
#include "../../parser/ast/type/primitive_type_ast_node.h"
#include "../../parser/ast/type_declaration/class_declaration_ast_node.h"
#include "../errors/errors.h"
#include "util.h"

#include <unordered_set>

static void check_return_type(
    const codesh::ast::compilation_unit_ast_node &root,
    const codesh::ast::method_declaration_ast_node *method,
    const std::string &class_name
);

static void check_parameters(
    const codesh::ast::compilation_unit_ast_node &root,
    const codesh::ast::method_declaration_ast_node *method,
    const std::string &class_name
);


void codesh::semantic_analyzer::check_methods(ast::compilation_unit_ast_node &root) {
    for (auto &type_decl : root.get_type_declarations())
    {
        auto *class_node = dynamic_cast<ast::type_decl::class_declaration_ast_node *>(type_decl.get());
        if (!class_node)
            continue;

        std::unordered_set<std::string> method_names;

        for (const auto &method : class_node->get_methods())
        {
            check_return_type(root, method.get(), class_node->get_name());
            check_parameters(root, method.get(), class_node->get_name());
        }
    }
}


static void check_return_type(
    const codesh::ast::compilation_unit_ast_node &root,
    const codesh::ast::method_declaration_ast_node *method,
    const std::string &class_name
) {
    const std::string &method_name = method->get_name();
    auto *return_type = method->get_return_type();

    const auto *custom_type =
        dynamic_cast<codesh::ast::type::custom_type_ast_node *>(return_type);

    if (!custom_type)
        return;

    const std::string &type_name = custom_type->get_name();

    if (!codesh::semantic_analyzer::util::type_exists(root, type_name))
    {
        codesh::semantic_analyzer::collect_error(
            "Unknown return type " + type_name +
            " in method " + method_name +
            " of type " + class_name
        );
    }
}

static void check_parameters(
    const codesh::ast::compilation_unit_ast_node &root,
    const codesh::ast::method_declaration_ast_node *method,
    const std::string &class_name
) {
    const std::string &method_name = method->get_name();

    for (const auto &param : method->get_parameters())
    {
        auto *param_type = param->get_type();

        if (dynamic_cast<codesh::ast::type::primitive_type_ast_node *>(param_type))
            continue;

        const auto *custom_param =
            dynamic_cast<codesh::ast::type::custom_type_ast_node *>(param_type);

        if (!custom_param)
        {
            codesh::semantic_analyzer::collect_error(
                "Invalid parameter type in method " + method_name +
                " of type " + class_name
            );
            continue;
        }

        const std::string &param_type_name = custom_param->get_name();

        if (!codesh::semantic_analyzer::util::type_exists(root, param_type_name))
        {
            codesh::semantic_analyzer::collect_error(
                "Unknown parameter type " + param_type_name +
                " in method " + method_name +
                " of type " + class_name
            );
        }
    }
}
