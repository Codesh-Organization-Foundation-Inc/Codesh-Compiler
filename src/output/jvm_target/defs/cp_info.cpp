#include "cp_info.h"

#include <boost/functional/hash.hpp>

codesh::output::jvm_target::defs::cp_info::cp_info(const unsigned char tag) : tag(tag)
{}

codesh::output::jvm_target::defs::cp_info::~cp_info() = default;


codesh::output::jvm_target::defs::CONSTANT_Utf8_info::CONSTANT_Utf8_info() :
    cp_info(1)
{}

codesh::output::jvm_target::defs::CONSTANT_Integer_info::CONSTANT_Integer_info() :
    cp_info(3)
{}
codesh::output::jvm_target::defs::CONSTANT_Float_info::CONSTANT_Float_info() :
    cp_info(4)
{}
codesh::output::jvm_target::defs::CONSTANT_Long_info::CONSTANT_Long_info() :
    cp_info(5)
{}
codesh::output::jvm_target::defs::CONSTANT_Double_info::CONSTANT_Double_info() :
    cp_info(6)
{}
codesh::output::jvm_target::defs::CONSTANT_Class_info::CONSTANT_Class_info() :
    cp_info(7)
{}
codesh::output::jvm_target::defs::CONSTANT_String_info::CONSTANT_String_info() :
    cp_info(8)
{}
codesh::output::jvm_target::defs::CONSTANT_Fieldref_info::CONSTANT_Fieldref_info() :
    cp_info(9)
{}
codesh::output::jvm_target::defs::CONSTANT_Methodref_info::CONSTANT_Methodref_info() :
    cp_info(10)
{}
codesh::output::jvm_target::defs::CONSTANT_InterfaceMethodref_info::CONSTANT_InterfaceMethodref_info() :
    cp_info(11)
{}
codesh::output::jvm_target::defs::CONSTANT_NameAndType_info::CONSTANT_NameAndType_info() :
    cp_info(12)
{}


// Functions for std::unordered_map

size_t codesh::output::jvm_target::defs::CONSTANT_Utf8_info::hash_code() const
{
    size_t seed = std::hash<unsigned char>()(tag);
    boost::hash_combine(seed, boost::hash_range(bytes.begin(), bytes.end()));
    return seed;
}

size_t codesh::output::jvm_target::defs::CONSTANT_Integer_info::hash_code() const
{
    size_t seed = std::hash<unsigned char>()(tag);
    boost::hash_combine(seed, boost::hash_range(bytes, bytes + 4));
    return seed;
}

size_t codesh::output::jvm_target::defs::CONSTANT_Float_info::hash_code() const
{
    size_t seed = std::hash<unsigned char>()(tag);
    boost::hash_combine(seed, boost::hash_range(bytes, bytes + 4));
    return seed;
}

size_t codesh::output::jvm_target::defs::CONSTANT_Long_info::hash_code() const
{
    size_t seed = std::hash<unsigned char>()(tag);
    boost::hash_combine(seed, boost::hash_range(high_bytes, high_bytes + 4));
    boost::hash_combine(seed, boost::hash_range(low_bytes, low_bytes + 4));
    return seed;
}

size_t codesh::output::jvm_target::defs::CONSTANT_Double_info::hash_code() const
{
    size_t seed = std::hash<unsigned char>()(tag);
    boost::hash_combine(seed, boost::hash_range(high_bytes, high_bytes + 4));
    boost::hash_combine(seed, boost::hash_range(low_bytes, low_bytes + 4));
    return seed;
}

size_t codesh::output::jvm_target::defs::CONSTANT_Class_info::hash_code() const
{
    size_t seed = std::hash<unsigned char>()(tag);
    boost::hash_combine(seed, boost::hash_range(name_index, name_index + 2));
    return seed;
}

size_t codesh::output::jvm_target::defs::CONSTANT_String_info::hash_code() const
{
    size_t seed = std::hash<unsigned char>()(tag);
    boost::hash_combine(seed, boost::hash_range(string_index, string_index + 2));
    return seed;
}

size_t codesh::output::jvm_target::defs::CONSTANT_Fieldref_info::hash_code() const
{
    size_t seed = std::hash<unsigned char>()(tag);
    boost::hash_combine(seed, boost::hash_range(class_index, class_index + 2));
    boost::hash_combine(seed, boost::hash_range(name_and_type_index, name_and_type_index + 2));
    return seed;
}

size_t codesh::output::jvm_target::defs::CONSTANT_Methodref_info::hash_code() const
{
    size_t seed = std::hash<unsigned char>()(tag);
    boost::hash_combine(seed, boost::hash_range(class_index, class_index + 2));
    boost::hash_combine(seed, boost::hash_range(name_and_type_index, name_and_type_index + 2));
    return seed;
}

size_t codesh::output::jvm_target::defs::CONSTANT_InterfaceMethodref_info::hash_code() const
{
    size_t seed = std::hash<unsigned char>()(tag);
    boost::hash_combine(seed, boost::hash_range(class_index, class_index + 2));
    boost::hash_combine(seed, boost::hash_range(name_and_type_index, name_and_type_index + 2));
    return seed;
}

size_t codesh::output::jvm_target::defs::CONSTANT_NameAndType_info::hash_code() const
{
    size_t seed = std::hash<unsigned char>()(tag);
    boost::hash_combine(seed, boost::hash_range(name_index, name_index + 2));
    boost::hash_combine(seed, boost::hash_range(descriptor_index, descriptor_index + 2));
    return seed;
}


bool codesh::output::jvm_target::defs::CONSTANT_Utf8_info::operator==(const CONSTANT_Utf8_info &other) const
{
    return tag == other.tag &&
           bytes == other.bytes;
}

bool codesh::output::jvm_target::defs::CONSTANT_Integer_info::operator==(const CONSTANT_Integer_info &other) const
{
    return tag == other.tag &&
           std::equal(bytes, bytes + 4, other.bytes);
}

bool codesh::output::jvm_target::defs::CONSTANT_Float_info::operator==(const CONSTANT_Float_info &other) const
{
    return tag == other.tag &&
           std::equal(bytes, bytes + 4, other.bytes);
}

bool codesh::output::jvm_target::defs::CONSTANT_Long_info::operator==(const CONSTANT_Long_info &other) const
{
    return tag == other.tag &&
           std::equal(high_bytes, high_bytes + 4, other.high_bytes) &&
           std::equal(low_bytes, low_bytes + 4, other.low_bytes);
}

bool codesh::output::jvm_target::defs::CONSTANT_Double_info::operator==(const CONSTANT_Double_info &other) const
{
    return tag == other.tag &&
           std::equal(high_bytes, high_bytes + 4, other.high_bytes) &&
           std::equal(low_bytes, low_bytes + 4, other.low_bytes);
}

bool codesh::output::jvm_target::defs::CONSTANT_Class_info::operator==(const CONSTANT_Class_info &other) const
{
    return tag == other.tag &&
           std::equal(name_index, name_index + 2, other.name_index);
}

bool codesh::output::jvm_target::defs::CONSTANT_String_info::operator==(const CONSTANT_String_info &other) const
{
    return tag == other.tag &&
           std::equal(string_index, string_index + 2, other.string_index);
}

bool codesh::output::jvm_target::defs::CONSTANT_Fieldref_info::operator==(const CONSTANT_Fieldref_info &other) const
{
    return tag == other.tag &&
           std::equal(class_index, class_index + 2, other.class_index) &&
           std::equal(name_and_type_index, name_and_type_index + 2, other.name_and_type_index);
}

bool codesh::output::jvm_target::defs::CONSTANT_Methodref_info::operator==(const CONSTANT_Methodref_info &other) const
{
    return tag == other.tag &&
           std::equal(class_index, class_index + 2, other.class_index) &&
           std::equal(name_and_type_index, name_and_type_index + 2, other.name_and_type_index);
}

bool codesh::output::jvm_target::defs::CONSTANT_InterfaceMethodref_info::operator==(const CONSTANT_InterfaceMethodref_info &other) const
{
    return tag == other.tag &&
           std::equal(class_index, class_index + 2, other.class_index) &&
           std::equal(name_and_type_index, name_and_type_index + 2, other.name_and_type_index);
}

bool codesh::output::jvm_target::defs::CONSTANT_NameAndType_info::operator==(const CONSTANT_NameAndType_info &other) const
{
    return tag == other.tag &&
           std::equal(name_index, name_index + 2, other.name_index) &&
           std::equal(descriptor_index, descriptor_index + 2, other.descriptor_index);
}



size_t codesh::output::jvm_target::defs::cp_info_ptr_hash::operator()(const std::unique_ptr<const cp_info> &obj) const
{
    return obj->hash_code();
}

bool codesh::output::jvm_target::defs::cp_info_ptr_equal::operator()(const std::unique_ptr<const cp_info> &lhs,
        const std::unique_ptr<const cp_info> &rhs) const
{
    if (lhs->tag != rhs->tag)
        return false;

    switch (lhs->tag)
    {
    case 1:
        return *static_cast<const CONSTANT_Utf8_info *>(lhs.get()) == // NOLINT(*-pro-type-static-cast-downcast)
               *static_cast<const CONSTANT_Utf8_info *>(rhs.get()); // NOLINT(*-pro-type-static-cast-downcast)
    case 3:
        return *static_cast<const CONSTANT_Integer_info *>(lhs.get()) == // NOLINT(*-pro-type-static-cast-downcast)
               *static_cast<const CONSTANT_Integer_info *>(rhs.get()); // NOLINT(*-pro-type-static-cast-downcast)
    case 4:
        return *static_cast<const CONSTANT_Float_info *>(lhs.get()) == // NOLINT(*-pro-type-static-cast-downcast)
               *static_cast<const CONSTANT_Float_info *>(rhs.get()); // NOLINT(*-pro-type-static-cast-downcast)
    case 5:
        return *static_cast<const CONSTANT_Long_info *>(lhs.get()) == // NOLINT(*-pro-type-static-cast-downcast)
               *static_cast<const CONSTANT_Long_info *>(rhs.get()); // NOLINT(*-pro-type-static-cast-downcast)
    case 6:
        return *static_cast<const CONSTANT_Double_info *>(lhs.get()) == // NOLINT(*-pro-type-static-cast-downcast)
               *static_cast<const CONSTANT_Double_info *>(rhs.get()); // NOLINT(*-pro-type-static-cast-downcast)
    case 7:
        return *static_cast<const CONSTANT_Class_info *>(lhs.get()) == // NOLINT(*-pro-type-static-cast-downcast)
               *static_cast<const CONSTANT_Class_info *>(rhs.get()); // NOLINT(*-pro-type-static-cast-downcast)
    case 8:
        return *static_cast<const CONSTANT_String_info *>(lhs.get()) == // NOLINT(*-pro-type-static-cast-downcast)
               *static_cast<const CONSTANT_String_info *>(rhs.get()); // NOLINT(*-pro-type-static-cast-downcast)
    case 9:
        return *static_cast<const CONSTANT_Fieldref_info *>(lhs.get()) == // NOLINT(*-pro-type-static-cast-downcast)
               *static_cast<const CONSTANT_Fieldref_info *>(rhs.get()); // NOLINT(*-pro-type-static-cast-downcast)
    case 10:
        return *static_cast<const CONSTANT_Methodref_info *>(lhs.get()) == // NOLINT(*-pro-type-static-cast-downcast)
               *static_cast<const CONSTANT_Methodref_info *>(rhs.get()); // NOLINT(*-pro-type-static-cast-downcast)
    case 11:
        return *static_cast<const CONSTANT_InterfaceMethodref_info *>(lhs.get()) == // NOLINT(*-pro-type-static-cast-downcast)
               *static_cast<const CONSTANT_InterfaceMethodref_info *>(rhs.get()); // NOLINT(*-pro-type-static-cast-downcast)
    case 12:
        return *static_cast<const CONSTANT_NameAndType_info *>(lhs.get()) == // NOLINT(*-pro-type-static-cast-downcast)
               *static_cast<const CONSTANT_NameAndType_info *>(rhs.get()); // NOLINT(*-pro-type-static-cast-downcast)
    default:
        return false;
    }
}
