#include "external_class_loader.h"

#include <cstdint>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "blasphemy/blasphemy_collector.h"
#include "defenition/fully_qualified_name.h"
#include "output/jvm_target/defs/cp_info.h"
#include "parser/ast/type_declaration/attributes_ast_node.h"
#include "semantic_analyzer/symbol_table/symbol_table.h"

/**
 * Maps a constant pool index to its associated string
 */
using cp_strings = std::unordered_map<int, std::string>;

using codesh::output::jvm_target::defs::constant_info_type;
using codesh::semantic_analyzer::country_symbol;
using codesh::semantic_analyzer::type_symbol;
using codesh::semantic_analyzer::field_symbol;
using codesh::semantic_analyzer::method_symbol;
using codesh::semantic_analyzer::method_overloads_symbol;
using codesh::semantic_analyzer::symbol_table;
using codesh::definition::fully_qualified_name;

static uint8_t read_u1(std::ifstream &file);
static uint16_t read_u2(std::ifstream &file);
static uint32_t read_u4(std::ifstream &file);
static void skip_attributes(std::ifstream &file, uint16_t count);
static std::string get_utf8(const cp_strings &strings, int idx);
static fully_qualified_name get_class_name(const cp_strings &strings, int idx);
static void parse_constant_pool(std::ifstream &file, cp_strings &strings);

static std::vector<fully_qualified_name> read_interface_names(std::ifstream &file, const cp_strings &strings);

static void read_magic(std::ifstream &file);
static type_symbol &parse_type(std::ifstream &file, const cp_strings &strings, const symbol_table &table);
static void parse_fields(std::ifstream &file, const cp_strings &strings, type_symbol &type_sym);
static void parse_methods(std::ifstream &file, const cp_strings &strings, type_symbol &type_sym);

static std::unique_ptr<codesh::ast::type_decl::attributes_ast_node> make_attributes_from_flags(uint16_t flags);
static country_symbol &find_or_create_country(const symbol_table &table, const std::string &package_name);


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

    type_symbol &type_sym = parse_type(file, strings, table);
    parse_fields(file, strings, type_sym);
    parse_methods(file, strings, type_sym);
}

static void parse_methods(std::ifstream &file, const cp_strings &strings, type_symbol &type_sym)
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

static type_symbol &parse_type(std::ifstream &file, const cp_strings &strings, const symbol_table &table)
{
    const auto access_flags = read_u2(file);
    const auto this_class_idx = read_u2(file);

    //TODO:
    // read_u2(file); // super_class_idx

    const auto class_name = get_class_name(strings, this_class_idx);

    //TODO:
    // read_interface_names(file, strings);

    country_symbol &country = find_or_create_country(table, class_name.omit_last().join());

    return country.get_scope().add_symbol(
        class_name.get_last_part(),
        std::make_unique<type_symbol>(
            &country,
            class_name,
            make_attributes_from_flags(access_flags),
            nullptr
        )
    ).first.get();
}

static void parse_fields(std::ifstream &file, const cp_strings &strings, type_symbol &type_sym)
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

static country_symbol &find_or_create_country(const symbol_table &table, const std::string &package_name)
{
    country_symbol &root = table.resolve_country("").value();

    if (package_name.empty())
        return root;

    // Walk each '/'-separated segment, creating country_symbols as needed
    country_symbol *current = &root;
    std::string accumulated;

    size_t start = 0;
    while (true)
    {
        const size_t slash_pos = package_name.find('/', start);
        const bool last = slash_pos == std::string::npos;
        const std::string part = package_name.substr(start, last ? std::string::npos : slash_pos - start);

        if (!accumulated.empty())
        {
            accumulated += '/';
        }
        accumulated += part;

        const auto existing = current->get_scope().resolve_local(part);
        if (existing)
        {
            current = &dynamic_cast<country_symbol &>(existing->get());
        }
        else
        {
            current = &current->get_scope().add_symbol(
                part,
                std::make_unique<country_symbol>(
                    accumulated.c_str(),
                    current
                )
            ).first.get();
        }

        if (last)
            break;

        start = slash_pos + 1;
    }

    return *current;
}

static std::unique_ptr<codesh::ast::type_decl::attributes_ast_node> make_attributes_from_flags(const uint16_t flags)
{
    auto result = std::make_unique<codesh::ast::type_decl::attributes_ast_node>(codesh::blasphemy::NO_CODE_POS);

    if (flags & 0x0001)
        result->set_visibility(codesh::definition::visibility::PUBLIC);
    else if (flags & 0x0002)
        result->set_visibility(codesh::definition::visibility::PRIVATE);
    else if (flags & 0x0004)
        result->set_visibility(codesh::definition::visibility::PROTECTED);
    // else: PACKAGE_PRIVATE (default)

    result->set_is_static(flags & 0x0008);
    result->set_is_final(flags & 0x0010);
    result->set_is_abstract((flags & 0x0400) != 0 || (flags & 0x0200) != 0); // ACC_ABSTRACT or ACC_INTERFACE

    return result;
}

static void parse_constant_pool(std::ifstream &file, cp_strings &strings)
{
    const uint16_t cp_count = read_u2(file);

    // CLASS_REF entries may forward-reference UTF8 entries that appear later in the constant pool.
    // Hence, wait for the FULL resolution until all entries are read.
    std::unordered_map<int, int> class_ref_indices;

    for (int i = 1; i < cp_count; i++)
    {
        uint8_t tag = read_u1(file);

        // We only care about UTF8 and CLASS_REF for the symbol table
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
            class_ref_indices.emplace(i, name_jvm_idx);

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
        case constant_info_type::DYNAMIC:
        case constant_info_type::INVOKE_DYNAMIC:
            read_u4(file);
            break;
        case constant_info_type::METHOD_HANDLE:
            read_u1(file);
            read_u2(file);
            break;
        case constant_info_type::METHOD_TYPE:
        case constant_info_type::MODULE:
        case constant_info_type::PACKAGE:
            read_u2(file);
            break;
        default:
            throw std::runtime_error("Unknown constant pool tag: " + std::to_string(tag));
        }
    }

    for (const auto &[class_idx, name_idx] : class_ref_indices)
    {
        strings.emplace(class_idx, get_utf8(strings, name_idx));
    }
}

static void read_magic(std::ifstream &file)
{
    if (read_u4(file) != 0xCAFEBABE)
        throw std::runtime_error("Not a valid .class file");
}

static std::vector<fully_qualified_name> read_interface_names(std::ifstream &file, const cp_strings &strings)
{
    const auto interfaces = read_u2(file);

    std::vector<fully_qualified_name> interface_names;
    interface_names.reserve(interfaces);

    for (size_t i = 0; i < interfaces; i++)
    {
        interface_names.push_back(get_class_name(strings, read_u2(file)));
    }

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

static fully_qualified_name get_class_name(const cp_strings &strings, const int idx)
{
    const auto it = strings.find(idx);
    if (it == strings.end())
        throw std::runtime_error("Constant pool index " + std::to_string(idx) + " is not a class entry");

    return it->second.c_str();
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
