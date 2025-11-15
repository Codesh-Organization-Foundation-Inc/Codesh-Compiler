#include "constant_pool.h"

#include "../../util.h"

#include <unordered_set>

#include "../../parser/ast/compilation_unit_ast_node.h"
#include "../../parser/ast/type_declaration/class_declaration_ast_node.h"

codesh::output::jvm_target::constant_pool::constant_pool(const ast::compilation_unit_ast_node &root_node) : index(1)
{
    add_utf8_constant("SourceFile");
    add_utf8_constant(root_node.get_source_stem() + ".אמן");

    // If there's at least a single class, there's code in it.
    if (!root_node.get_type_declarations().empty())
    {
        add_utf8_constant("<init>");

        add_utf8_constant("Code");
        add_utf8_constant("LocalVariableTable");
        add_utf8_constant("this");

        traverse_type_decls(root_node);
    }
}

void codesh::output::jvm_target::constant_pool::add_constant(std::unique_ptr<defs::cp_info> root_node)
{
    const auto [it, inserted] = literals.try_emplace(std::move(root_node), index);

    if (inserted)
    {
        literals_lookup_map.emplace(it->first.get(), index);
        index++;
    }
}

void codesh::output::jvm_target::constant_pool::add_utf8_constant(const std::string &utf8)
{
    if (utf8.size() > 0xFFFF)
        throw std::runtime_error("String size is longer than possible; max length is 65535");

    auto utf8_info = std::make_unique<defs::CONSTANT_Utf8_info>();
    util::put_int_bytes(utf8_info->length, 2, utf8.length()); // NOLINT(*-narrowing-conversions) (Handled overflow above)
    utf8_info->bytes.insert(utf8_info->bytes.end(), utf8.begin(), utf8.end());

    add_constant(std::move(utf8_info));
}

int codesh::output::jvm_target::constant_pool::get_index(const defs::cp_info &literal) const
{
    const auto result = literals_lookup_map.find(&literal);
    if (result == literals_lookup_map.end())
        return -1;

    return result->second;
}

std::vector<std::reference_wrapper<const codesh::output::jvm_target::defs::cp_info>>
    codesh::output::jvm_target::constant_pool::get_literals() const
{
    // The literals need to match the order written in the Class file.
    // The order is specified by this map's int value.
    // So make the value the new key, and return the sorted vector.
    //
    // This then means that every return index (with get_index) matches the one in the class file.

    std::unordered_map<int, const defs::cp_info *> inverted_literals;
    for (const auto &[key, value] : literals_lookup_map)
    {
        inverted_literals.emplace(value, key);
    }


    std::vector<std::reference_wrapper<const defs::cp_info>> results;
    results.reserve(inverted_literals.size());

    for (int i = 0; i < inverted_literals.size(); i++)
    {
        results.emplace_back(*inverted_literals.at(i + 1));
    }

    return results;
}

void codesh::output::jvm_target::constant_pool::traverse_type_decls(const ast::compilation_unit_ast_node &root_node)
{
    for (const auto &type_decl : root_node.get_type_declarations())
    {
        add_utf8_constant(type_decl->get_binary_name());
        add_utf8_constant(type_decl->generate_descriptor());

        if (const auto class_decl = dynamic_cast<const ast::type_decl::class_declaration_ast_node *>(type_decl.get()))
        {
            traverse_class_decl(*class_decl);
        }
    }
}

void codesh::output::jvm_target::constant_pool::traverse_class_decl(
        const ast::type_decl::class_declaration_ast_node &class_decl_node)
{
    const auto super_class = class_decl_node.get_super_class();

    if (super_class == nullptr)
    {
        // If it doesn't extend anything, it extends Object.
        add_utf8_constant("java/lang/Object");
    }
    else
    {
        add_utf8_constant(super_class->generate_descriptor());
    }

    //TODO: This should only happen if no constructor is present
    add_utf8_constant("()V");
}
