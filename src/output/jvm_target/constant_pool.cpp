#include "constant_pool.h"

#include <unordered_set>

#include "parser/ast/compilation_unit_ast_node.h"


codesh::output::jvm_target::constant_pool::constant_pool(const ast::compilation_unit_ast_node &root_node,
        ast::type_decl::type_declaration_ast_node &type_decl) :
    index(1)
{
    type_decl.emit_constants(root_node, *this);
}

int codesh::output::jvm_target::constant_pool::goc_constant(std::unique_ptr<defs::cp_info> constant_info)
{
    // Long and Double entries occupy two constant pool slots
    const bool is_wide = dynamic_cast<const defs::CONSTANT_Long_info *>(constant_info.get())
                      || dynamic_cast<const defs::CONSTANT_Double_info *>(constant_info.get());

    const auto [it, inserted] = literals.emplace(std::move(constant_info));

    if (inserted)
    {
        literals_lookup_map.emplace(it->get(), index);

        const int assigned_index = index;
        index += is_wide ? 2 : 1;
        return assigned_index;
    }

    return literals_lookup_map.at(it->get());
}

int codesh::output::jvm_target::constant_pool::goc_utf8_info(const std::string &utf8)
{
    return goc_constant(std::make_unique<defs::CONSTANT_Utf8_info>(utf8));
}

int codesh::output::jvm_target::constant_pool::goc_string_info(const int utf8_index)
{
    return goc_constant(std::make_unique<defs::CONSTANT_String_info>(utf8_index));
}

int codesh::output::jvm_target::constant_pool::goc_integer_info(const int num)
{
    return goc_constant(std::make_unique<defs::CONSTANT_Integer_info>(num));
}

int codesh::output::jvm_target::constant_pool::goc_float_info(const float num)
{
    return goc_constant(std::make_unique<defs::CONSTANT_Float_info>(num));
}

int codesh::output::jvm_target::constant_pool::goc_long_info(const long long num)
{
    return goc_constant(std::make_unique<defs::CONSTANT_Long_info>(num));
}

int codesh::output::jvm_target::constant_pool::goc_double_info(const double num)
{
    return goc_constant(std::make_unique<defs::CONSTANT_Double_info>(num));
}

int codesh::output::jvm_target::constant_pool::goc_methodref_info(const int class_index,
        const int name_and_type_index)
{
    return goc_constant(std::make_unique<defs::CONSTANT_Methodref_info>(class_index, name_and_type_index));
}

int codesh::output::jvm_target::constant_pool::goc_name_and_type_info(const int name_index,
        const int descriptor_index)
{
    return goc_constant(std::make_unique<defs::CONSTANT_NameAndType_info>(name_index, descriptor_index));
}

int codesh::output::jvm_target::constant_pool::goc_class_info(const int name_index)
{
    return goc_constant(std::make_unique<defs::CONSTANT_Class_info>(name_index));
}

int codesh::output::jvm_target::constant_pool::goc_fieldref_info(const int class_index,
        const int name_and_type_index)
{
    return goc_constant(std::make_unique<defs::CONSTANT_Fieldref_info>(class_index, name_and_type_index));
}

int codesh::output::jvm_target::constant_pool::get_index(const defs::cp_info &literal) const
{
    const auto result = literals_lookup_map.find(&literal);
    if (result == literals_lookup_map.end())
        throw std::invalid_argument("Could not find literal in constant pool");

    return result->second;
}

int codesh::output::jvm_target::constant_pool::get_utf8_index(const std::string &utf8) const
{
    return get_index(defs::CONSTANT_Utf8_info(utf8));
}

int codesh::output::jvm_target::constant_pool::get_string_index(const int utf8_index) const
{
    return get_index(defs::CONSTANT_String_info(utf8_index));
}

int codesh::output::jvm_target::constant_pool::get_integer_index(const int num) const
{
    return get_index(defs::CONSTANT_Integer_info(num));
}

int codesh::output::jvm_target::constant_pool::get_float_index(const float num) const
{
    return get_index(defs::CONSTANT_Float_info(num));
}

int codesh::output::jvm_target::constant_pool::get_long_index(const long long num) const
{
    return get_index(defs::CONSTANT_Long_info(num));
}

int codesh::output::jvm_target::constant_pool::get_double_index(const double num) const
{
    return get_index(defs::CONSTANT_Double_info(num));
}

int codesh::output::jvm_target::constant_pool::get_methodref_index(const int class_index,
        const int name_and_type_index) const
{
    return get_index(defs::CONSTANT_Methodref_info(class_index, name_and_type_index));
}

int codesh::output::jvm_target::constant_pool::get_name_and_type_index(const int name_index,
        const int descriptor_index) const
{
    return get_index(defs::CONSTANT_NameAndType_info(name_index, descriptor_index));
}

int codesh::output::jvm_target::constant_pool::get_class_index(const int name_index) const
{
    return get_index(defs::CONSTANT_Class_info(name_index));
}

int codesh::output::jvm_target::constant_pool::get_fieldref_index(const int class_index,
        const int name_and_type_index) const
{
    return get_index(defs::CONSTANT_Fieldref_info(class_index, name_and_type_index));
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

    for (int i = 1; i < index; i++)
    {
        const auto it = inverted_literals.find(i);
        if (it == inverted_literals.end())
        {
            // Phantom slot after a Long/Double entry; skip it.
            continue;
        }
        results.emplace_back(*it->second);
    }

    return results;
}

size_t codesh::output::jvm_target::constant_pool::size() const
{
    // index starts at 1 and points one past the last used slot,
    // so (index - 1) is the total number of slots including phantom
    // slots consumed by Long/Double entries.
    return index - 1;
}
