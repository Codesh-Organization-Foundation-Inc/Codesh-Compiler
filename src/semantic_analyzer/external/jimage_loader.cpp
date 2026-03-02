#include "jimage_loader.h"
#include "class_loader.h"
#include "util.h"
#include "defenition/fully_qualified_name.h"

#include <cstring>
#include <optional>
#include <fstream>
#include <vector>

namespace util = codesh::semantic_analyzer::external::util;

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

enum class jimage_attribute
{
    END,
    MODULE,
    PARENT,
    BASE,
    EXTENSION,
    OFFSET,
    COMPRESSED
};

static jimage_offsets parse_header(std::ifstream &file);
static std::vector<int32_t> load_redirect_table(std::ifstream &file, const jimage_offsets &layout);
static std::vector<uint32_t> load_offsets(std::ifstream &file, const jimage_offsets &layout);
static std::vector<char> load_strings(std::ifstream &file, const jimage_offsets &layout);
static std::optional<uint64_t> load_target_class_offset(std::ifstream &file, const jimage_offsets &layout,
        const std::string &target_class);
static uint16_t read_u2_le(std::ifstream &file);
static uint32_t read_u4_le(std::ifstream &file);
static uint64_t read_attr_value(std::ifstream &file, int size);


//TIL: streamoff is an int representing file offsets
//TODO: Maybe convert all file offsets to use streamoff if others exist
static constexpr std::streamoff HEADER_SIZE = 24;


bool codesh::semantic_analyzer::external::load_jimage_class(const std::filesystem::path &path,
        const definition::fully_qualified_name &class_name, const symbol_table &table)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open " + path.string());
    }

    const auto layout = parse_header(file);

    const auto target_class_offset = load_target_class_offset(
        file,
        layout,
        class_name.join()
    );

    if (!target_class_offset.has_value())
        return false;


    file.seekg(layout.data_start + static_cast<std::streamoff>(target_class_offset.value()));
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

static std::vector<char> load_strings(std::ifstream &file, const jimage_offsets &layout)
{
    file.seekg(layout.strings_start);

    std::vector<char> strings(layout.strings_size);
    file.read(strings.data(), layout.strings_size);

    return strings;
}

static std::optional<uint64_t> load_target_class_offset(std::ifstream &file, const jimage_offsets &layout,
        const std::string &target_class)
{
    const auto offsets = load_offsets(file, layout);
    const auto strings = load_strings(file, layout);
    const auto redirections = load_redirect_table(file, layout);

    const auto redirect_index = util::jimage_perfect_hash_index(target_class, layout.table_length);
    const auto redirect_value = redirections[redirect_index];

    if (redirect_value > 0)
    {
        // Direct slot
        return static_cast<uint64_t>(redirect_value) - 1;
    }

    // Indirect slot; requires 2nd lookup where the seed is the current redirect value
    return util::jimage_perfect_hash_index(target_class, layout.table_length, -redirect_value - 1);
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

// Location attribute values are big-endian even inside the LE jimage format
static uint64_t read_attr_value(std::ifstream &file, int const size)
{
    uint64_t value = 0;
    for (int i = 0; i < size; ++i)
        value = value << 8 | util::read_u1(file);
    return value;
}
