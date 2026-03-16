#include "class_loader.h"

#include "../util.h"
#include "util.h"

#include <cstdint>
#include <fstream>
#include <istream>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "parser/ast/type/custom_type_ast_node.h"

#include "blasphemy/blasphemy_collector.h"
#include "defenition/fully_qualified_name.h"
#include "output/jvm_target/defs/cp_info.h"
#include "parser/ast/type_declaration/attributes_ast_node.h"
#include "semantic_analyzer/symbol_table/symbol_table.h"

/**
 * Maps a constant pool index to its associated string
 */
using cp_strings = std::unordered_map<int, std::string>;

namespace util = codesh::semantic_analyzer::external::util;

using codesh::output::jvm_target::defs::constant_info_type;
using codesh::semantic_analyzer::country_symbol;
using codesh::semantic_analyzer::type_symbol;
using codesh::semantic_analyzer::field_symbol;
using codesh::semantic_analyzer::method_symbol;
using codesh::semantic_analyzer::method_overloads_symbol;
using codesh::semantic_analyzer::symbol_table;
using codesh::definition::fully_qualified_name;

static void skip_attributes(std::istream &file, uint16_t count);
static std::string get_utf8(const cp_strings &strings, int idx);
static fully_qualified_name get_class_name(const cp_strings &strings, int idx);
static void parse_constant_pool(std::istream &file, cp_strings &strings);

static std::vector<fully_qualified_name> read_interface_names(std::istream &file, const cp_strings &strings);

static void read_magic(std::istream &file);
static type_symbol &parse_type(std::istream &file, const cp_strings &strings, const symbol_table &table);
static void parse_fields(std::istream &file, const cp_strings &strings, type_symbol &type_sym);
static void parse_methods(std::istream &file, const cp_strings &strings, type_symbol &type_sym);

static std::unique_ptr<codesh::ast::type_decl::attributes_ast_node> flags_to_attributes(uint16_t flags);
static std::unique_ptr<codesh::ast::type::type_ast_node> descriptor_to_node_type(const std::string &descriptor,
        size_t &pos);

static void add_method_symbol(const std::string &method_descriptor, const std::string &method_name,
        std::unique_ptr<codesh::ast::type_decl::attributes_ast_node> attributes, type_symbol &type_sym);


//TODO: Convert all errors to blasphemies
void codesh::semantic_analyzer::external::load_class_file(
        const std::filesystem::path &path, const symbol_table &table)
{
    std::ifstream file(path, std::ios::binary);
    if (!file)
        throw std::runtime_error("Cannot open: " + path.string());

    load_class_file(file, table);
}

void codesh::semantic_analyzer::external::load_class_file(std::istream &file, const symbol_table &table)
{
    read_magic(file);
    util::read_u2(file); // minor_version
    util::read_u2(file); // major_version

    cp_strings strings;
    parse_constant_pool(file, strings);

    type_symbol &type_sym = parse_type(file, strings, table);
    parse_fields(file, strings, type_sym);
    parse_methods(file, strings, type_sym);
}

static void parse_methods(std::istream &file, const cp_strings &strings, type_symbol &type_sym)
{
    const auto methods_count = util::read_u2(file);
    for (size_t i = 0; i < methods_count; i++)
    {
        auto access_flags = flags_to_attributes(util::read_u2(file));
        const auto method_name_idx = util::read_u2(file);
        const auto method_desc_idx = util::read_u2(file);
        const auto method_attr_count = util::read_u2(file);

        skip_attributes(file, method_attr_count);

        const auto method_name = get_utf8(strings, method_name_idx);
        const auto method_descriptor = get_utf8(strings, method_desc_idx);

        add_method_symbol(
            method_descriptor,
            method_name,
            std::move(access_flags),
            type_sym
        );
    }
}

static void add_method_symbol(const std::string &method_descriptor, const std::string &method_name,
        std::unique_ptr<codesh::ast::type_decl::attributes_ast_node> attributes, type_symbol &type_sym)
{
    // Parse descriptor "(param1param2...)return_type"
    std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> param_types;

    size_t pos = 1; // '('
    while (pos < method_descriptor.size() && method_descriptor.at(pos) != ')')
    {
        param_types.push_back(descriptor_to_node_type(method_descriptor, pos));
    }
    pos++; // ')'

    auto return_type = descriptor_to_node_type(method_descriptor, pos);


    method_overloads_symbol &overloads = codesh::semantic_analyzer::util::get_method_overloads_symbol(
        method_name,
        type_sym
    );

    overloads.get_scope().add_symbol(
        method_descriptor,
        std::make_unique<method_symbol>(
            &overloads,
            type_sym,
            type_sym.get_full_name().with(method_name),
            std::move(attributes),
            std::move(param_types),
            std::move(return_type),
            nullptr
        )
    );
}

static type_symbol &parse_type(std::istream &file, const cp_strings &strings, const symbol_table &table)
{
    auto access_flags = flags_to_attributes(util::read_u2(file));
    const auto this_class_idx = util::read_u2(file);
    const auto super_class_idx = util::read_u2(file);

    const auto class_name = get_class_name(strings, this_class_idx);

    std::unique_ptr<codesh::ast::type::custom_type_ast_node> super_type_node;
    if (super_class_idx != 0)
    {
        const auto super_class_name = get_class_name(strings, super_class_idx);

        super_type_node = std::make_unique<codesh::ast::type::custom_type_ast_node>(
            codesh::blasphemy::NO_CODE_POS,
            super_class_name
        );
    }

    const auto interfaces = read_interface_names(file, strings);
    std::vector<std::unique_ptr<codesh::ast::type::custom_type_ast_node>> interface_nodes;
    interface_nodes.reserve(interfaces.size());
    for (const auto &interface_name : interfaces)
    {
        interface_nodes.push_back(std::make_unique<codesh::ast::type::custom_type_ast_node>(
            codesh::blasphemy::NO_CODE_POS,
            interface_name
        ));
    }

    country_symbol &country = codesh::semantic_analyzer::util::find_or_create_country(
        table,
        class_name.omit_last().join()
    );

    return codesh::semantic_analyzer::util::add_type_symbol(
        country,
        class_name.get_last_part(),
        std::move(access_flags),
        std::move(super_type_node),
        std::move(interface_nodes)
    ).first.get();
}

static void parse_fields(std::istream &file, const cp_strings &strings, type_symbol &type_sym)
{
    const auto fields_count = util::read_u2(file);
    for (size_t fi = 0; fi < fields_count; fi++)
    {
        auto attributes = flags_to_attributes(util::read_u2(file));
        const auto field_name_idx = util::read_u2(file);
        const auto field_desc_idx = util::read_u2(file);
        const auto field_attr_count = util::read_u2(file);

        skip_attributes(file, field_attr_count);

        const auto field_name = get_utf8(strings, field_name_idx);
        const auto field_descriptor = get_utf8(strings, field_desc_idx);

        size_t pos = 0;
        codesh::semantic_analyzer::util::add_field_symbol(
            type_sym,
            field_name,
            std::move(attributes),
            descriptor_to_node_type(field_descriptor, pos)
        );
    }
}


static std::unique_ptr<codesh::ast::type_decl::attributes_ast_node> flags_to_attributes(const uint16_t flags)
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

static std::unique_ptr<codesh::ast::type::type_ast_node> descriptor_to_node_type(const std::string &descriptor,
        size_t &pos)
{
    return codesh::ast::type::type_ast_node::from_descriptor(descriptor, pos, codesh::blasphemy::NO_CODE_POS);
}

static void parse_constant_pool(std::istream &file, cp_strings &strings)
{
    const uint16_t cp_count = util::read_u2(file);

    // CLASS_REF entries may forward-reference UTF8 entries that appear later in the constant pool.
    // Hence, wait for the FULL resolution until all entries are read.
    std::unordered_map<int, int> class_ref_indices;

    for (int i = 1; i < cp_count; i++)
    {
        uint8_t tag = util::read_u1(file);

        // We only care about UTF8 and CLASS_REF for the symbol table
        switch (static_cast<constant_info_type>(tag))
        {
        case constant_info_type::UTF8: {
            const uint16_t len = util::read_u2(file);
            std::string utf8(len, '\0');
            file.read(utf8.data(), len);

            strings.emplace(i, utf8);

            break;
        }
        case constant_info_type::CLASS_REF: {
            const int name_jvm_idx = util::read_u2(file);
            class_ref_indices.emplace(i, name_jvm_idx);

            break;
        }

        case constant_info_type::INTEGER:
        case constant_info_type::FLOATING:
            util::read_u4(file);
            break;
        case constant_info_type::LONG_INT:
        case constant_info_type::DOUBLE_FP:
            util::read_u4(file);
            util::read_u4(file);
            i++; // Skip phantom slot
            break;
        case constant_info_type::STRING_REF:
            util::read_u2(file);
            break;
        case constant_info_type::FIELDREF:
        case constant_info_type::METHODREF:
        case constant_info_type::INTERFACE_METHODREF:
        case constant_info_type::NAME_AND_TYPE:
        case constant_info_type::DYNAMIC:
        case constant_info_type::INVOKE_DYNAMIC:
            util::read_u4(file);
            break;
        case constant_info_type::METHOD_HANDLE:
            util::read_u1(file);
            util::read_u2(file);
            break;
        case constant_info_type::METHOD_TYPE:
        case constant_info_type::MODULE:
        case constant_info_type::PACKAGE:
            util::read_u2(file);
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

static void read_magic(std::istream &file)
{
    if (util::read_u4(file) != 0xCAFEBABE)
        throw std::runtime_error("Not a valid .class file");
}

static std::vector<fully_qualified_name> read_interface_names(std::istream &file, const cp_strings &strings)
{
    const auto interfaces = util::read_u2(file);

    std::vector<fully_qualified_name> interface_names;
    interface_names.reserve(interfaces);

    for (size_t i = 0; i < interfaces; i++)
    {
        interface_names.push_back(get_class_name(strings, util::read_u2(file)));
    }

    return interface_names;
}

static void skip_attributes(std::istream &file, const uint16_t count)
{
    for (uint16_t a = 0; a < count; a++)
    {
        util::read_u2(file);
        file.seekg(util::read_u4(file), std::ios::cur);
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

    return fully_qualified_name::parse(it->second, codesh::blasphemy::NO_CODE_POS);
}
