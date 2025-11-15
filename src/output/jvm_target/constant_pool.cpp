#include "constant_pool.h"

#include <unordered_set>

#include "../../parser/ast/compilation_unit_ast_node.h"
#include "../../parser/ast/type_declaration/class_declaration_ast_node.h"

codesh::output::jvm_target::constant_pool::constant_pool(const ast::compilation_unit_ast_node &root_node)
{
    // We must first collect all literals before collecting all others as other pools depend on the string literals.
    collect_literals(root_node);
    collect_non_literals(root_node);
}

int codesh::output::jvm_target::constant_pool::get_index(const std::string &literal) const
{
    const auto result = string_literals.find(literal);
    if (result == string_literals.end())
        return -1;

    return result->second;
}

std::ranges::elements_view<std::ranges::ref_view<const std::map<std::string, int>>, 0>
    codesh::output::jvm_target::constant_pool::get_string_literals() const
{
    return std::views::keys(string_literals);
}

// TODO: Support other non-string literals
void codesh::output::jvm_target::constant_pool::collect_literals(const ast::compilation_unit_ast_node &root_node)
{
    std::unordered_set<std::string> literals = {
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


    int index = 1;
    for (const auto &literal : literals)
    {
        string_literals.emplace(literal, index++);
    }
}

void codesh::output::jvm_target::constant_pool::traverse_type_decls(const ast::compilation_unit_ast_node &root_node,
        std::unordered_set<std::string> &literals)
{
    for (const auto &type_decl : root_node.get_type_declarations())
    {
        literals.emplace(type_decl->get_binary_name());
        literals.emplace(type_decl->generate_descriptor());

        if (const auto class_decl = dynamic_cast<const ast::type_decl::class_declaration_ast_node *>(type_decl.get()))
        {
            traverse_class_decl(*class_decl, literals);
        }
    }
}

void codesh::output::jvm_target::constant_pool::traverse_class_decl(
        const ast::type_decl::class_declaration_ast_node &class_decl_node, std::unordered_set<std::string> &literals)
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


void codesh::output::jvm_target::constant_pool::collect_non_literals(const ast::compilation_unit_ast_node &root_node)
{
}