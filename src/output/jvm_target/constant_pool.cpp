#include "constant_pool.h"

#include <cstring>
#include <unordered_set>

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "util.h"
#include "parser/ast/compilation_unit_ast_node.h"

codesh::output::jvm_target::constant_pool::constant_pool(const ast::compilation_unit_ast_node &root_node,
        ast::type_decl::type_declaration_ast_node &type_decl) :
    index(1)
{
    type_decl.emit_constants(root_node, *this);
}

std::unique_ptr<codesh::output::jvm_target::defs::CONSTANT_Utf8_info>
    codesh::output::jvm_target::constant_pool::utf8_info(const std::string &utf8)
{
    if (utf8.size() > 0xFFFF)
    {
        blasphemy::blasphemy_collector().add_blasphemy(blasphemy::details::STRING_TOO_BIG,
            blasphemy::blasphemy_type::OUTPUT, blasphemy::NO_CODE_POS, true);
    }

    auto utf8_info = std::make_unique<defs::CONSTANT_Utf8_info>();

    util::put_int_bytes(utf8_info->length, 2, utf8.length()); // NOLINT(*-narrowing-conversions) (Handled overflow above)
    utf8_info->bytes.insert(utf8_info->bytes.end(), utf8.begin(), utf8.end());

    return utf8_info;
}

std::unique_ptr<codesh::output::jvm_target::defs::CONSTANT_String_info> codesh::output::jvm_target::constant_pool::
    string_info(const int utf8_index)
{
    auto const_string = std::make_unique<defs::CONSTANT_String_info>();

    util::put_int_bytes(const_string->string_index, 2, utf8_index);

    return const_string;
}

std::unique_ptr<codesh::output::jvm_target::defs::CONSTANT_Integer_info> codesh::output::jvm_target::constant_pool::
    integer_info(const int num)
{
    auto const_integer = std::make_unique<defs::CONSTANT_Integer_info>();

    util::put_int_bytes(const_integer->bytes, 4, num);

    return const_integer;
}

std::unique_ptr<codesh::output::jvm_target::defs::CONSTANT_Float_info> codesh::output::jvm_target::constant_pool::
    float_info(const float num)
{
    auto const_float = std::make_unique<defs::CONSTANT_Float_info>();

    uint32_t bits;
    std::memcpy(&bits, &num, sizeof(bits));
    util::put_int_bytes(const_float->bytes, 4, static_cast<int>(bits));

    return const_float;
}

std::unique_ptr<codesh::output::jvm_target::defs::CONSTANT_Long_info> codesh::output::jvm_target::constant_pool::
    long_info(const long long num)
{
    auto const_long = std::make_unique<defs::CONSTANT_Long_info>();

    util::put_int_bytes(const_long->high_bytes, 4, static_cast<int>(num >> 32));
    util::put_int_bytes(const_long->low_bytes, 4, static_cast<int>(num & 0xFFFFFFFF));

    return const_long;
}

std::unique_ptr<codesh::output::jvm_target::defs::CONSTANT_Double_info> codesh::output::jvm_target::constant_pool::
    double_info(const double num)
{
    auto const_double = std::make_unique<defs::CONSTANT_Double_info>();

    uint64_t bits;
    std::memcpy(&bits, &num, sizeof(bits));
    util::put_int_bytes(const_double->high_bytes, 4, static_cast<int>(bits >> 32));
    util::put_int_bytes(const_double->low_bytes, 4, static_cast<int>(bits & 0xFFFFFFFF));

    return const_double;
}

std::unique_ptr<codesh::output::jvm_target::defs::CONSTANT_Methodref_info>
    codesh::output::jvm_target::constant_pool::methodref_info(const int class_index, const int name_and_type_index)
{
    auto const_methodref = std::make_unique<defs::CONSTANT_Methodref_info>();

    util::put_int_bytes(const_methodref->class_index, 2, class_index);
    util::put_int_bytes(const_methodref->name_and_type_index, 2, name_and_type_index);

    return const_methodref;
}

std::unique_ptr<codesh::output::jvm_target::defs::CONSTANT_NameAndType_info>
    codesh::output::jvm_target::constant_pool::name_and_type_info(const int name_index, const int descriptor_index)
{
    auto const_name_and_type = std::make_unique<defs::CONSTANT_NameAndType_info>();

    util::put_int_bytes(const_name_and_type->name_index, 2, name_index);
    util::put_int_bytes(const_name_and_type->descriptor_index, 2, descriptor_index);

    return const_name_and_type;
}

std::unique_ptr<codesh::output::jvm_target::defs::CONSTANT_Class_info>
    codesh::output::jvm_target::constant_pool::class_info(const int name_index)
{
    auto const_class = std::make_unique<defs::CONSTANT_Class_info>();

    util::put_int_bytes(const_class->name_index, 2, name_index);

    return const_class;
}

std::unique_ptr<codesh::output::jvm_target::defs::CONSTANT_Fieldref_info> codesh::output::jvm_target::constant_pool::
    fieldref_info(const int class_index, const int name_and_type_index)
{
    auto const_fieldref = std::make_unique<defs::CONSTANT_Fieldref_info>();

    util::put_int_bytes(const_fieldref->class_index, 2, class_index);
    util::put_int_bytes(const_fieldref->name_and_type_index, 2, name_and_type_index);

    return const_fieldref;
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
    return goc_constant(utf8_info(utf8));
}

int codesh::output::jvm_target::constant_pool::goc_string_info(const int utf8_index)
{
    return goc_constant(string_info(utf8_index));
}

int codesh::output::jvm_target::constant_pool::goc_integer_info(const int num)
{
    return goc_constant(integer_info(num));
}

int codesh::output::jvm_target::constant_pool::goc_float_info(const float num)
{
    return goc_constant(float_info(num));
}

int codesh::output::jvm_target::constant_pool::goc_long_info(const long long num)
{
    return goc_constant(long_info(num));
}

int codesh::output::jvm_target::constant_pool::goc_double_info(const double num)
{
    return goc_constant(double_info(num));
}

int codesh::output::jvm_target::constant_pool::goc_methodref_info(const int class_index, const int name_and_type_index)
{
    return goc_constant(methodref_info(class_index, name_and_type_index));
}
int codesh::output::jvm_target::constant_pool::goc_name_and_type_info(const int name_index, const int descriptor_index)
{
    return goc_constant(name_and_type_info(name_index, descriptor_index));
}
int codesh::output::jvm_target::constant_pool::goc_class_info(const int name_index)
{
    return goc_constant(class_info(name_index));
}
int codesh::output::jvm_target::constant_pool::goc_fieldref_info(const int class_index, const int name_and_type_index)
{
    return goc_constant(fieldref_info(class_index, name_and_type_index));
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
    return get_index(*utf8_info(utf8));
}

int codesh::output::jvm_target::constant_pool::get_string_index(const int utf8_index) const
{
    return get_index(*string_info(utf8_index));
}

int codesh::output::jvm_target::constant_pool::get_integer_index(const int num) const
{
    return get_index(*integer_info(num));
}

int codesh::output::jvm_target::constant_pool::get_float_index(const float num) const
{
    return get_index(*float_info(num));
}

int codesh::output::jvm_target::constant_pool::get_long_index(const long long num) const
{
    return get_index(*long_info(num));
}

int codesh::output::jvm_target::constant_pool::get_double_index(const double num) const
{
    return get_index(*double_info(num));
}

int codesh::output::jvm_target::constant_pool::get_methodref_index(const int class_index,
                                                                   const int name_and_type_index) const
{
    return get_index(*methodref_info(class_index, name_and_type_index));
}

int codesh::output::jvm_target::constant_pool::get_name_and_type_index(const int name_index,
                                                                       const int descriptor_index) const
{
    return get_index(*name_and_type_info(name_index, descriptor_index));
}

int codesh::output::jvm_target::constant_pool::get_class_index(const int name_index) const
{
    return get_index(*class_info(name_index));
}

int codesh::output::jvm_target::constant_pool::get_fieldref_index(const int class_index,
                                                                  const int name_and_type_index) const
{
    return get_index(*fieldref_info(class_index, name_and_type_index));
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
