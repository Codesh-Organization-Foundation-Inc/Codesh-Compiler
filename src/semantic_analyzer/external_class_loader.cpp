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

using cp_map = std::unordered_map<int, std::unique_ptr<codesh::output::jvm_target::defs::cp_info>>;
using constant_info_type = codesh::output::jvm_target::defs::constant_info_type;

static uint8_t read_u1(std::ifstream &file);
static uint16_t read_u2(std::ifstream &file);
static uint32_t read_u4(std::ifstream &file);
static cp_map parse_constant_pool(std::ifstream &file);

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

    cp_map pool = parse_constant_pool(file);
}

static cp_map parse_constant_pool(std::ifstream &file)
{
    cp_map pool;

    const uint16_t cp_count = read_u2(file);
    for (int i = 1; i < cp_count; i++)
    {
        uint8_t tag = read_u1(file);

        switch (static_cast<constant_info_type>(tag))
        {
        case constant_info_type::UTF8: {
            const uint16_t len = read_u2(file);
            std::string utf8(len, '\0');
            file.read(utf8.data(), len);

            pool.emplace(i, std::make_unique<codesh::output::jvm_target::defs::CONSTANT_Utf8_info>(utf8));

            break;
        }
        case constant_info_type::INTEGER: {
            pool.emplace(i, std::make_unique<codesh::output::jvm_target::defs::CONSTANT_Integer_info>(
                static_cast<int>(read_u4(file))
            ));

            break;
        }
        case constant_info_type::FLOATING: {
            uint32_t bits_len = read_u4(file);
            float num;
            std::memcpy(&num, &bits_len, sizeof(num));

            pool.emplace(i, std::make_unique<codesh::output::jvm_target::defs::CONSTANT_Float_info>(num));

            break;
        }
        case constant_info_type::LONG_INT: {
            const uint32_t high = read_u4(file);
            const uint32_t low = read_u4(file);

            pool.emplace(i, std::make_unique<codesh::output::jvm_target::defs::CONSTANT_Long_info>(
                static_cast<long long>(high) << 32 | low
            ));

            i++;
            break;
        }
        case constant_info_type::DOUBLE_FP: {
            const uint32_t high = read_u4(file);
            const uint32_t low = read_u4(file);

            uint64_t bits_len = static_cast<uint64_t>(high) << 32 | low;
            double num;
            std::memcpy(&num, &bits_len, sizeof(num));
            pool.emplace(i, std::make_unique<codesh::output::jvm_target::defs::CONSTANT_Double_info>(num));

            i++;
            break;
        }
        case constant_info_type::CLASS_REF: {
            pool.emplace(i, std::make_unique<codesh::output::jvm_target::defs::CONSTANT_Class_info>(
                read_u2(file)
            ));

            break;
        }
        case constant_info_type::STRING_REF: {
            pool.emplace(i, std::make_unique<codesh::output::jvm_target::defs::CONSTANT_String_info>(
                read_u2(file)
            ));

            break;
        }
        case constant_info_type::FIELDREF: {
            pool.emplace(i, std::make_unique<codesh::output::jvm_target::defs::CONSTANT_Fieldref_info>(
                read_u2(file),
                read_u2(file)
            ));

            break;
        }
        case constant_info_type::METHODREF: {
            pool.emplace(i, std::make_unique<codesh::output::jvm_target::defs::CONSTANT_Methodref_info>(
                read_u2(file),
                read_u2(file)
            ));

            break;
        }
        case constant_info_type::INTERFACE_METHODREF: {
            pool.emplace(i, std::make_unique<codesh::output::jvm_target::defs::CONSTANT_InterfaceMethodref_info>(
                read_u2(file),
                read_u2(file)
            ));

            break;
        }
        case constant_info_type::NAME_AND_TYPE: {
            pool.emplace(i, std::make_unique<codesh::output::jvm_target::defs::CONSTANT_NameAndType_info>(
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
