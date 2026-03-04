#include "jimage_loader.h"
#include "class_loader.h"
#include "defenition/fully_qualified_name.h"
#include "fmt/base.h"
#include "fmt/xchar.h"
#include "util.h"

#include <fstream>
#include <optional>
#include <vector>

/*
 * Thank you o mighty lord and savior OpenJDK for being a free and open
 * i will forever be in debt
 * xoxo
 * https://github.com/openjdk/jdk/blob/master/src/java.base/share/native/libjimage/imageFile.hpp
 */

namespace util = codesh::semantic_analyzer::external::util;
using codesh::semantic_analyzer::external::jimage_location_attribute;

// Keep JImage offsets to skip only the relevant parts using ifstream::seekg and skip everything else
struct jimage_offsets
{
    // header_size is constantly HEADER_SIZE
    uint32_t table_length;
    uint32_t locations_size;
    uint32_t strings_size;
    std::streamoff offsets_start;
    std::streamoff locations_start;
    std::streamoff strings_start;
    std::streamoff data_start;
};

struct location_result
{
    uint32_t index;
    uint64_t size;
};

[[nodiscard]] static jimage_offsets parse_header(std::ifstream &file);
[[nodiscard]] static std::vector<int32_t> load_redirect_table(std::ifstream &file, const jimage_offsets &layout);
[[nodiscard]] static std::vector<uint32_t> load_offsets(std::ifstream &file, const jimage_offsets &layout);
[[nodiscard]] static std::vector<char> load_strings(std::ifstream &file, const jimage_offsets &layout);
[[nodiscard]] static std::vector<unsigned char> load_locations(std::ifstream &file, const jimage_offsets &layout);
[[nodiscard]] static std::optional<int32_t> get_location_offset_index(std::ifstream &file, const jimage_offsets &layout,
        const std::string &target_class);
[[nodiscard]] static std::optional<location_result> load_classfile_offset(std::ifstream &file,
        const jimage_offsets &layout, const std::string &module_name, const std::string &target_class);
static uint16_t read_u2_le(std::ifstream &file);
static uint32_t read_u4_le(std::ifstream &file);

// TIL: streamoff is an int representing file offsets
//TODO: Maybe convert all file offsets to use streamoff if others exist
static constexpr std::streamoff HEADER_SIZE = 28;


bool codesh::semantic_analyzer::external::load_jimage_class(const std::filesystem::path &path,
        const std::string &module_name, const definition::fully_qualified_name &class_name, const symbol_table &table)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open " + path.string());
    }

    const auto layout = parse_header(file);

    const auto target_class_offset = load_classfile_offset(
        file,
        layout,
        module_name,
        class_name.join()
    );

    if (!target_class_offset.has_value())
        return false;


    const auto class_file_offset = layout.data_start + static_cast<std::streamoff>(target_class_offset->index);
    fmt::println("[JIMAGE] {}", class_file_offset);

    file.seekg(layout.data_start + static_cast<std::streamoff>(class_file_offset));
    load_class_file(file, table);
    return true;
}

static jimage_offsets parse_header(std::ifstream &file)
{
    if (read_u4_le(file) != 0xCAFEDADA)
    {
        throw std::runtime_error("Invalid JImage");
    }

    read_u2_le(file); // major_version
    read_u2_le(file); // minor_version
    read_u4_le(file); // flags
    read_u4_le(file); // resource_count

    const auto table_length = read_u4_le(file);
    const auto locations_size = read_u4_le(file);
    const auto strings_size = read_u4_le(file);

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

static std::optional<location_result> load_classfile_offset(std::ifstream &file, const jimage_offsets &layout,
        const std::string &module_name, const std::string &target_class)
{
    const auto path = fmt::format("/{}/{}.class", module_name, target_class);
    const auto offset_index = get_location_offset_index(file, layout, path);

    if (!offset_index.has_value())
        return std::nullopt;

    const auto location_offset = load_offsets(file, layout).at(*offset_index);

    const auto locations = load_locations(file, layout);
    const auto strings = load_strings(file, layout);

    if (!util::verify_location(locations, strings, location_offset, path))
        return std::nullopt;

    return location_result {
        static_cast<uint32_t>(*offset_index),
        util::read_location_attribute(locations, location_offset, jimage_location_attribute::UNCOMPRESSED)
    };
}


static std::vector<int32_t> load_redirect_table(std::ifstream &file, const jimage_offsets &layout)
{
    file.seekg(HEADER_SIZE);

    std::vector<int32_t> redirect(layout.table_length);
    for (int32_t &entry : redirect)
    {
        entry = static_cast<int32_t>(read_u4_le(file));
    }

    return redirect;
}

static std::vector<uint32_t> load_offsets(std::ifstream &file, const jimage_offsets &layout)
{
    file.seekg(layout.offsets_start);

    std::vector<uint32_t> offsets(layout.table_length);
    for (uint32_t &entry : offsets)
    {
        entry = read_u4_le(file);
    }

    return offsets;
}

static std::vector<unsigned char> load_locations(std::ifstream &file, const jimage_offsets &layout)
{
    file.seekg(layout.locations_start);

    std::vector<unsigned char> locations(layout.locations_size);
    file.read(reinterpret_cast<char *>(locations.data()), layout.locations_size);

    return locations;
}

static std::vector<char> load_strings(std::ifstream &file, const jimage_offsets &layout)
{
    file.seekg(layout.strings_start);

    std::vector<char> strings(layout.strings_size);
    file.read(strings.data(), layout.strings_size);

    return strings;
}

static std::optional<int32_t> get_location_offset_index(std::ifstream &file, const jimage_offsets &layout,
        const std::string &target_class)
{
    const auto redirections = load_redirect_table(file, layout);

    const auto redirect_index = util::jimage_perfect_hash_index(target_class, layout.table_length);
    const auto redirect_value = redirections[redirect_index];

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
        return util::jimage_perfect_hash_index(target_class, layout.table_length, redirect_value);
    }

    // Not a class file
    return std::nullopt;
}


// Re-implemented because reverse order matters here
static uint16_t read_u2_le(std::ifstream &file)
{
    const uint16_t lo = util::read_u1(file);
    const uint16_t hi = util::read_u1(file);
    return static_cast<uint16_t>(lo | hi << 8);
}

static uint32_t read_u4_le(std::ifstream &file)
{
    const uint32_t lo = read_u2_le(file);
    const uint32_t hi = read_u2_le(file);
    return lo | hi << 16;
}
