#include "external_class_loader.h"

#include <cstdint>
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
}

static void read_magic(std::ifstream &file)
{
    if (read_u4(file) != 0xCAFEBABE)
        throw std::runtime_error("Not a valid .class file");
}


static uint8_t read_u1(std::ifstream &file)
{
    uint8_t b;
    file.read(reinterpret_cast<char *>(&b), 1);
    return b;
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

