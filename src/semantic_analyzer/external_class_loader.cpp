#include "external_class_loader.h"

#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "output/jvm_target/defs/cp_info.h"
#include "semantic_analyzer/symbol_table/symbol_table.h"

/**
 * Maps a constant pool index to its associated string
 */
using cp_strings = std::unordered_map<int, std::string>;

using constant_info_type = codesh::output::jvm_target::defs::constant_info_type;

static uint8_t read_u1(std::ifstream &file);
static uint16_t read_u2(std::ifstream &file);
static uint32_t read_u4(std::ifstream &file);
static void skip_attributes(std::ifstream &file, uint16_t count);
static std::string get_utf8(const cp_strings &strings, int idx);
static std::string get_class_name(const cp_strings &strings, int idx);
static void parse_constant_pool(std::ifstream &file, cp_strings &strings);

static std::vector<std::string> read_interface_names(std::ifstream &file, const cp_strings &strings);

static void read_magic(std::ifstream &file);
static void parse_type(std::ifstream &file, const cp_strings &strings);
static void parse_fields(std::ifstream &file, const cp_strings &strings);
static void parse_methods(std::ifstream &file, const cp_strings &strings);


//TODO: Convert all errors to blasphemies
void codesh::semantic_analyzer::load_external_class_file(
        const std::filesystem::path &path, symbol_table &table)
{
    std::ifstream file(path, std::ios::binary);
    if (!file)
        throw std::runtime_error("Cannot open: " + path.string());

    read_magic(file);
    read_u2(file); // minor_version
    read_u2(file); // major_version

    cp_strings strings;
    parse_constant_pool(file, strings);

    parse_type(file, strings);
    parse_fields(file, strings);
    parse_methods(file, strings);

    (void)table;
}

static void parse_methods(std::ifstream &file, const cp_strings &strings)
{
    const auto methods_count = read_u2(file);
    for (size_t i = 0; i < methods_count; i++)
    {
        const auto method_access_flags = read_u2(file);
        const auto method_name_idx = read_u2(file);
        const auto method_desc_idx = read_u2(file);
        const auto method_attr_count = read_u2(file);

        skip_attributes(file, method_attr_count);

        const auto method_name = get_utf8(strings, method_name_idx);
        const auto method_descriptor = get_utf8(strings, method_desc_idx);

        // TODO: Register method_symbol for this method.
        // Available variables:
        //   std::string method_name        — e.g. "length", "<init>", "<clinit>"
        //   std::string method_descriptor  — e.g. "()I", "(Ljava/lang/String;I)V"
        //                                    Format: "(" + param_descriptors + ")" + return_descriptor
        //   uint16_t    method_access_flags— raw JVM access flags
        //   (type_symbol for the enclosing class is what you registered above)
        (void)method_access_flags;
        (void)method_name;
        (void)method_descriptor;
    }
}

static void parse_type(std::ifstream &file, const cp_strings &strings)
{
    const auto access_flags = read_u2(file);
    const auto this_class_idx = read_u2(file);
    const auto super_class_idx = read_u2(file);
    const auto is_interface = (access_flags & 0x0200) != 0;

    const auto class_name = get_class_name(strings, this_class_idx);
    const auto super_class_name = super_class_idx != 0 ? get_class_name(strings, super_class_idx) : "";

    const auto interface_names = read_interface_names(file, strings);

    // TODO: Register type_symbol for this type
    // Available variables:
    //   std::string        class_name        — e.g. "java/lang/String"
    //   std::string        super_class_name  — e.g. "java/lang/Object", or "" for Object itself
    //   std::vector<std::string> interface_names — e.g. {"java/io/Serializable", "java/lang/Comparable"}
    //   uint16_t           access_flags      — raw JVM access flags (ACC_PUBLIC=0x0001, ACC_INTERFACE=0x0200, ACC_ABSTRACT=0x0400, ACC_FINAL=0x0010, ...)
    //   bool               is_interface      — shorthand for (access_flags & 0x0200)
    //   symbol_table&      table             — the symbol table to register into

    (void)is_interface;
}

static void parse_fields(std::ifstream &file, const cp_strings &strings)
{
    const auto fields_count = read_u2(file);
    for (size_t fi = 0; fi < fields_count; fi++)
    {
        const auto field_access_flags = read_u2(file);
        const auto field_name_idx = read_u2(file);
        const auto field_desc_idx = read_u2(file);
        const auto field_attr_count = read_u2(file);

        skip_attributes(file, field_attr_count);

        const auto field_name = get_utf8(strings, field_name_idx);
        const auto field_descriptor = get_utf8(strings, field_desc_idx);

        // TODO: Register field_symbol for this field
        // Available variables:
        //   std::string field_name        — e.g. "hash"
        //   std::string field_descriptor  — e.g. "I", "Ljava/lang/String;", "[B"
        //   uint16_t    field_access_flags— raw JVM access flags
        //   (type_symbol for the enclosing class is what you registered above)
        (void)field_access_flags;
        (void)field_name;
        (void)field_descriptor;
    }
}

static void parse_constant_pool(std::ifstream &file, cp_strings &strings)
{
    const uint16_t cp_count = read_u2(file);

    for (int i = 1; i < cp_count; i++)
    {
        uint8_t tag = read_u1(file);

        // We only care about Only UTF8 and CLASS_REF for the symbol table
        switch (static_cast<constant_info_type>(tag))
        {
        case constant_info_type::UTF8: {
            const uint16_t len = read_u2(file);
            std::string utf8(len, '\0');
            file.read(utf8.data(), len);

            strings.emplace(i, utf8);

            break;
        }
        case constant_info_type::CLASS_REF: {
            const int name_jvm_idx = read_u2(file);

            strings.emplace(i, strings[name_jvm_idx]);

            break;
        }

        case constant_info_type::INTEGER:
        case constant_info_type::FLOATING:
            read_u4(file);
            break;
        case constant_info_type::LONG_INT:
        case constant_info_type::DOUBLE_FP:
            read_u4(file);
            read_u4(file);
            i++; // Skip phantom slot
            break;
        case constant_info_type::STRING_REF:
            read_u2(file);
            break;
        case constant_info_type::FIELDREF:
        case constant_info_type::METHODREF:
        case constant_info_type::INTERFACE_METHODREF:
        case constant_info_type::NAME_AND_TYPE:
            read_u4(file);
            break;
        default:
            throw std::runtime_error("Unknown constant pool tag: " + std::to_string(tag));
        }
    }
}

static void read_magic(std::ifstream &file)
{
    if (read_u4(file) != 0xCAFEBABE)
        throw std::runtime_error("Not a valid .class file");
}

static std::vector<std::string> read_interface_names(std::ifstream &file, const cp_strings &strings)
{
    const auto interfaces = read_u2(file);

    std::vector<std::string> interface_names;
    interface_names.reserve(interfaces);

    for (size_t i = 0; i < interfaces; i++)
        interface_names.push_back(get_class_name(strings, read_u2(file)));

    return interface_names;
}

static void skip_attributes(std::ifstream &file, const uint16_t count)
{
    for (uint16_t a = 0; a < count; a++)
    {
        read_u2(file);
        file.seekg(read_u4(file), std::ios::cur);
    }
}

static std::string get_utf8(const cp_strings &strings, const int idx)
{
    const auto it = strings.find(idx);
    if (it == strings.end())
        throw std::runtime_error("Constant pool index " + std::to_string(idx) + " is not a UTF8 entry");

    return it->second;
}

static std::string get_class_name(const cp_strings &strings, const int idx)
{
    const auto it = strings.find(idx);
    if (it == strings.end())
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
