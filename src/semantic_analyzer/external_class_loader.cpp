#include "external_class_loader.h"

#include <cstdint>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "output/jvm_target/defs/cp_info.h"
#include "semantic_analyzer/symbol_table/symbol_table.h"

using cp_map = std::vector<std::unique_ptr<codesh::output::jvm_target::defs::cp_info>>;
/**
 * Maps a constant pool index to its associated string
 */
using cp_strings = std::unordered_map<int, std::string>;

using constant_info_type = codesh::output::jvm_target::defs::constant_info_type;

static uint8_t read_u1(std::ifstream &file);
static uint16_t read_u2(std::ifstream &file);
static uint32_t read_u4(std::ifstream &file);
static std::string get_utf8(const cp_strings &cp_strings, int idx);
static std::string get_class_name(const cp_strings &cp_strings, int idx);
static cp_map parse_constant_pool(std::ifstream &file, cp_strings &cp_strings);
static std::vector<std::string> read_interface_names(std::ifstream &file, const cp_strings &cp_strings);

static void read_magic(std::ifstream &file);


//TODO: Convert all errors to blasphemies
void load_external_class_file(const std::filesystem::path &path, codesh::semantic_analyzer::symbol_table &table)
{
    std::ifstream file(path, std::ios::binary);
    if (!file)
        throw std::runtime_error("Cannot open: " + path.string());

    read_magic(file);
    read_u2(file); // minor_version
    read_u2(file); // major_version

    cp_strings cp_strings;
    //TODO: Review if necessary to be saved
    const auto pool = parse_constant_pool(file, cp_strings);


    const auto access_flags = read_u2(file);
    const auto this_class_idx = read_u2(file);
    const auto super_class_idx = read_u2(file);
    const auto is_interface = (access_flags & 0x0200) != 0;


    const auto class_name = get_class_name(cp_strings, this_class_idx);
    const auto super_class_name = super_class_idx != 0 ? get_class_name(cp_strings, super_class_idx) : "";

    const auto interface_names = read_interface_names(file, cp_strings);


    // TODO: Register type_symbol for this class/interface
    // Available variables:
    //   std::string        class_name        — e.g. "java/lang/String"
    //   std::string        super_class_name  — e.g. "java/lang/Object", or "" for Object itself
    //   std::vector<std::string> interface_names — e.g. {"java/io/Serializable", "java/lang/Comparable"}
    //   uint16_t           access_flags      — raw JVM access flags (ACC_PUBLIC=0x0001, ACC_INTERFACE=0x0200, ACC_ABSTRACT=0x0400, ACC_FINAL=0x0010, ...)
    //   bool               is_interface      — shorthand for (access_flags & 0x0200)
    //   symbol_table&      table             — the symbol table to register into

    (void)access_flags;
    (void)is_interface;
    (void)class_name;
    (void)super_class_name;
    (void)interface_names;
    (void)table;
}

static cp_map parse_constant_pool(std::ifstream &file, cp_strings &cp_strings)
{
    const uint16_t cp_count = read_u2(file);

    cp_map pool;
    pool.reserve(cp_count);
    pool.emplace_back(nullptr); // index 0 unused (CP is 1-based)

    for (int i = 1; i < cp_count; i++)
    {
        uint8_t tag = read_u1(file);

        switch (static_cast<constant_info_type>(tag))
        {
        case constant_info_type::UTF8: {
            const uint16_t len = read_u2(file);
            std::string utf8(len, '\0');
            file.read(utf8.data(), len);

            cp_strings.emplace(i, utf8);
            pool.emplace_back(std::make_unique<codesh::output::jvm_target::defs::CONSTANT_Utf8_info>(utf8));

            break;
        }
        case constant_info_type::INTEGER: {
            pool.emplace_back(std::make_unique<codesh::output::jvm_target::defs::CONSTANT_Integer_info>(
                static_cast<int>(read_u4(file))
            ));

            break;
        }
        case constant_info_type::FLOATING: {
            uint32_t bits_len = read_u4(file);
            float num;
            std::memcpy(&num, &bits_len, sizeof(num));

            pool.emplace_back(std::make_unique<codesh::output::jvm_target::defs::CONSTANT_Float_info>(num));

            break;
        }
        case constant_info_type::LONG_INT: {
            const uint32_t high = read_u4(file);
            const uint32_t low = read_u4(file);

            pool.emplace_back(std::make_unique<codesh::output::jvm_target::defs::CONSTANT_Long_info>(
                static_cast<long long>(high) << 32 | low
            ));
            pool.emplace_back(nullptr); // Phantom slot

            i++;
            break;
        }
        case constant_info_type::DOUBLE_FP: {
            const uint32_t high = read_u4(file);
            const uint32_t low = read_u4(file);

            uint64_t bits_len = static_cast<uint64_t>(high) << 32 | low;
            double num;
            std::memcpy(&num, &bits_len, sizeof(num));

            pool.emplace_back(std::make_unique<codesh::output::jvm_target::defs::CONSTANT_Double_info>(num));
            pool.emplace_back(nullptr); // Phantom slot

            i++;
            break;
        }
        case constant_info_type::CLASS_REF: {
            const int name_jvm_idx = read_u2(file);

            cp_strings.emplace(i, cp_strings[name_jvm_idx]); // The class name should already be stored in the strings
            pool.emplace_back(std::make_unique<codesh::output::jvm_target::defs::CONSTANT_Class_info>(name_jvm_idx));

            break;
        }
        case constant_info_type::STRING_REF: {
            pool.emplace_back(std::make_unique<codesh::output::jvm_target::defs::CONSTANT_String_info>(
                read_u2(file)
            ));

            break;
        }
        case constant_info_type::FIELDREF: {
            pool.emplace_back(std::make_unique<codesh::output::jvm_target::defs::CONSTANT_Fieldref_info>(
                read_u2(file),
                read_u2(file)
            ));

            break;
        }
        case constant_info_type::METHODREF: {
            pool.emplace_back(std::make_unique<codesh::output::jvm_target::defs::CONSTANT_Methodref_info>(
                read_u2(file),
                read_u2(file)
            ));

            break;
        }
        case constant_info_type::INTERFACE_METHODREF: {
            pool.emplace_back(std::make_unique<codesh::output::jvm_target::defs::CONSTANT_InterfaceMethodref_info>(
                read_u2(file),
                read_u2(file)
            ));

            break;
        }
        case constant_info_type::NAME_AND_TYPE: {
            pool.emplace_back(std::make_unique<codesh::output::jvm_target::defs::CONSTANT_NameAndType_info>(
                read_u2(file),
                read_u2(file)
            ));

            break;
        }
        default:
            throw std::runtime_error("Unknown constant pool tag: " + std::to_string(tag));
        }
    }

    return pool;
}

static void read_magic(std::ifstream &file)
{
    if (read_u4(file) != 0xCAFEBABE)
        throw std::runtime_error("Not a valid .class file");
}

static std::vector<std::string> read_interface_names(std::ifstream &file, const cp_strings &cp_strings)
{
    const auto interfaces = read_u2(file);

    std::vector<std::string> interface_names;
    interface_names.reserve(interfaces);

    for (size_t i = 0; i < interfaces; i++)
    {
        interface_names.push_back(get_class_name(cp_strings, read_u2(file)));
    }

    return interface_names;
}


static std::string get_utf8(const cp_strings &cp_strings, const int idx)
{
    const auto it = cp_strings.find(idx);
    if (it == cp_strings.end())
        throw std::runtime_error("Constant pool index " + std::to_string(idx) + " is not a UTF8 entry");

    return it->second;
}

static std::string get_class_name(const cp_strings &cp_strings, const int idx)
{
    const auto it = cp_strings.find(idx);
    if (it == cp_strings.end())
        throw std::runtime_error("Constant pool index " + std::to_string(idx) + " is not a class entry");

    return it->second;
}


static uint8_t read_u1(std::ifstream &file)
{
    uint8_t byte;
    file.read(reinterpret_cast<char *>(&byte), 1);
    return byte;
}

static uint16_t read_u2(std::ifstream &file)
{
    const uint16_t high = read_u1(file);
    const uint16_t low = read_u1(file);
    return static_cast<uint16_t>(high << 8 | low);
}

static uint32_t read_u4(std::ifstream &file)
{
    const uint32_t high = read_u2(file);
    const uint32_t low = read_u2(file);
    return high << 16 | low;
}
