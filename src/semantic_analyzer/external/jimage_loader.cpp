#include "jimage_loader.h"

#include "class_loader.h"
#include "defenition/fully_qualified_name.h"
#include "fmt/base.h"
#include "fmt/xchar.h"
#include "util.h"

#include <fstream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <vector>

/*
 * Thank you o mighty lord and savior OpenJDK for being a free and open software
 * i will forever be in debt
 * xoxo
 * https://github.com/openjdk/jdk/blob/master/src/java.base/share/native/libjimage/imageFile.hpp
 */

namespace util = codesh::semantic_analyzer::external::util;
using codesh::semantic_analyzer::external::jimage_location_attribute;
using codesh::semantic_analyzer::external::jimage_loader;

jimage_loader::jimage_loader(const std::filesystem::path &path) : _file(path, std::ios::binary)
{
    if (!_file.is_open())
        throw std::runtime_error("Could not open " + path.string());

    _layout = parse_header();
    _redirect_table = load_redirect_table();
    _offsets = load_offsets();
    _locations = load_locations();
    _strings = load_strings();
}

jimage_loader::~jimage_loader()
{
    _file.close();
}

bool jimage_loader::load(const std::string &module_name, const definition::fully_qualified_name &class_name,
        const symbol_table &table)
{
    const auto lookup = lookup_class_file(module_name, class_name.join());
    if (!lookup.has_value())
        return false;

    const auto file_offset = _layout.data_start + static_cast<std::streamoff>(lookup->data_offset);

    if (lookup->compressed_size == 0)
    {
        _file.seekg(file_offset);
        load_class_file(_file, table);
    }
    else
    {
        load_compressed_class_file(file_offset, *lookup, table);
    }

    return true;
}

codesh::semantic_analyzer::external::jimage_offsets jimage_loader::parse_header()
{
    if (read_u4_le() != 0xCAFEDADA)
    {
        throw std::runtime_error("Invalid JImage");
    }

    read_u2_le(); // major_version
    read_u2_le(); // minor_version
    read_u4_le(); // flags
    read_u4_le(); // resource_count

    const auto table_length = read_u4_le();
    const auto locations_size = read_u4_le();
    const auto strings_size = read_u4_le();

    const auto offsets_start = HEADER_SIZE + static_cast<std::streamoff>(table_length) * 4;
    const auto locations_start = HEADER_SIZE + static_cast<std::streamoff>(table_length) * 8;
    const auto strings_start = locations_start + locations_size;
    const auto data_start = strings_start + strings_size;

    return jimage_offsets {
        table_length,
        locations_size,
        strings_size,
        offsets_start,
        locations_start,
        strings_start,
        data_start
    };
}

std::optional<codesh::semantic_analyzer::external::class_file_lookup_result> jimage_loader::lookup_class_file(
        const std::string &module_name, const std::string &target_class) const
{
    const auto path = fmt::format("/{}/{}.class", module_name, target_class);
    const auto offset_index = get_location_offset_index(path);

    if (!offset_index.has_value())
        return std::nullopt;


    const auto location_offset = _offsets.at(*offset_index);
    if (!util::verify_location(_locations, _strings, location_offset, path))
        return std::nullopt;


    return class_file_lookup_result {
        util::read_location_attribute(_locations, location_offset, jimage_location_attribute::OFFSET),
        util::read_location_attribute(_locations, location_offset, jimage_location_attribute::COMPRESSED),
        util::read_location_attribute(_locations, location_offset, jimage_location_attribute::UNCOMPRESSED)
    };
}

std::vector<int32_t> jimage_loader::load_redirect_table()
{
    _file.seekg(HEADER_SIZE);

    std::vector<int32_t> redirect(_layout.table_length);
    for (int32_t &entry : redirect)
    {
        entry = static_cast<int32_t>(read_u4_le());
    }

    return redirect;
}

std::vector<uint32_t> jimage_loader::load_offsets()
{
    _file.seekg(_layout.offsets_start);

    std::vector<uint32_t> offsets(_layout.table_length);
    for (uint32_t &entry : offsets)
    {
        entry = read_u4_le();
    }

    return offsets;
}

std::vector<unsigned char> jimage_loader::load_locations()
{
    _file.seekg(_layout.locations_start);

    std::vector<unsigned char> locations(_layout.locations_size);
    _file.read(reinterpret_cast<char *>(locations.data()), _layout.locations_size);

    return locations;
}

std::vector<char> jimage_loader::load_strings()
{
    _file.seekg(_layout.strings_start);

    std::vector<char> strings(_layout.strings_size);
    _file.read(strings.data(), _layout.strings_size);

    return strings;
}

std::optional<int32_t> jimage_loader::get_location_offset_index(const std::string &path) const
{
    const auto redirect_index = util::jimage_perfect_hash_index(path, _layout.table_length);
    const auto redirect_value = _redirect_table[redirect_index];

    // ReSharper disable once CppDFAConstantConditions
    if (redirect_value < 0)
    {
        // Direct slot
        // ReSharper disable once CppDFAUnreachableCode
        return -redirect_value - 1;
    }
    if (redirect_value > 0)
    {
        // Indirect slot; requires 2nd lookup where the seed is the current redirect value
        return util::jimage_perfect_hash_index(path, _layout.table_length, redirect_value);
    }

    // Not a class file
    return std::nullopt;
}


void jimage_loader::load_compressed_class_file(const std::streamoff file_offset,
        const class_file_lookup_result &lookup, const symbol_table &table)
{
    std::vector<uint8_t> compressed(lookup.compressed_size);
    _file.seekg(file_offset);
    _file.read(reinterpret_cast<char *>(compressed.data()), static_cast<std::streamsize>(lookup.compressed_size));

    const auto uncompressed = util::decompress_resource(compressed, lookup.size, _strings);

    std::string buf(uncompressed.begin(), uncompressed.end());
    std::istringstream stream(std::move(buf), std::ios::binary);
    load_class_file(stream, table);
}


uint16_t jimage_loader::read_u2_le()
{
    const uint16_t lo = util::read_u1(_file);
    const uint16_t hi = util::read_u1(_file);
    return static_cast<uint16_t>(lo | hi << 8);
}

uint32_t jimage_loader::read_u4_le()
{
    const uint32_t lo = read_u2_le();
    const uint32_t hi = read_u2_le();
    return lo | hi << 16;
}
