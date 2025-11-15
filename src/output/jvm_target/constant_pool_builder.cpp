#include "constant_pool_builder.h"

#include <set>

#include "../../parser/ast/compilation_unit_ast_node.h"
#include "../../parser/ast/type_declaration/class_declaration_ast_node.h"

static void traverse_type_decls(const codesh::ast::compilation_unit_ast_node &root_node, std::set<std::string> &literals);
static void traverse_class_decl(const codesh::ast::type_decl::class_declaration_ast_node &class_decl_node,
        std::set<std::string> &literals);


std::unordered_map<std::string, int> codesh::output::jvm_target::constant_pool_builder::build(
    const ast::compilation_unit_ast_node &root_node)
{
    std::set<std::string> literals = {
        "SourceFile",
        root_node.get_source_stem() + ".אמן"
    };


    // If there's at least a single class, there's code in it.
    if (!root_node.get_type_declarations().empty())
    {
        literals.emplace("<init>");

        literals.emplace("Code");
        literals.emplace("LocalVariableTable");
        literals.emplace("this");

        traverse_type_decls(root_node, literals);
    }


    std::unordered_map<std::string, int> results;

    int index = 1;
    for (const auto &literal : literals)
    {
        results.emplace(literal, index++);
    }

    return results;
}

static void traverse_type_decls(const codesh::ast::compilation_unit_ast_node &root_node, std::set<std::string> &literals)
{
    for (const auto &type_decl : root_node.get_type_declarations())
    {
        literals.emplace(type_decl->get_binary_name());
        literals.emplace(type_decl->generate_descriptor());

        if (const auto class_decl = dynamic_cast<const codesh::ast::type_decl::class_declaration_ast_node *>(type_decl.get()))
        {
            traverse_class_decl(*class_decl, literals);
        }
    }
}

static void traverse_class_decl(const codesh::ast::type_decl::class_declaration_ast_node &class_decl_node,
        std::set<std::string> &literals)
{
    const auto super_class = class_decl_node.get_super_class();

    if (super_class == nullptr)
    {
        // If it doesn't extend anything, it extends Object.
        literals.emplace("java/lang/Object");
    }
    else
    {
        literals.emplace(super_class->generate_descriptor());
    }

    //TODO: This should only happen if no constructor is present
    literals.emplace("()V");
}
