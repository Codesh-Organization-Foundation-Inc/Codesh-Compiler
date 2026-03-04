#include "util.h"

#include "jimage_loader.h"

#include <fstream>
#include <stdexcept>
#include <string_view>

#include <zlib.h>

namespace util = codesh::semantic_analyzer::external::util;
using codesh::semantic_analyzer::external::jimage_location_attribute;


struct location_string_offsets
{
    uint64_t module;
    uint64_t parent;
    uint64_t base;
    uint64_t extension;
};


int32_t jimage_perfect_hash(const std::string &str, int32_t seed);
/**
 * Pointer past matched prefix, or nullptr on mismatch.
 */
[[nodiscard]] static const char* path_starts_with(const char* str, const char* prefix);

/**
 * Null-terminated string at @p offset in the strings table.
 */
[[nodiscard]] static const char* get_string_at(const std::vector<char> &strings, uint64_t offset);

[[nodiscard]] static location_string_offsets parse_offsets(
        const std::vector<unsigned char> &location_bytes, uint32_t location_offset);


uint8_t codesh::semantic_analyzer::external::util::read_u1(std::istream &file)
{
    uint8_t byte;
    file.read(reinterpret_cast<char *>(&byte), 1);
    return byte;
}

uint16_t codesh::semantic_analyzer::external::util::read_u2(std::istream &file)
{
    const uint16_t high = read_u1(file);
    const uint16_t low = read_u1(file);
    return static_cast<uint16_t>(high << 8 | low);
}

uint32_t codesh::semantic_analyzer::external::util::read_u4(std::istream &file)
{
    const uint32_t high = read_u2(file);
    const uint32_t low = read_u2(file);
    return high << 16 | low;
}


int32_t codesh::semantic_analyzer::external::util::jimage_perfect_hash_index(const std::string &str,
        const uint32_t table_length, const int32_t seed)
{
    return jimage_perfect_hash(str, seed) % static_cast<int32_t>(table_length);
}

int32_t jimage_perfect_hash(const std::string &str, const int32_t seed)
{
    // Copied from: https://github.com/openjdk/jdk/blob/86800eb2b34bd6ea7a77e7a9ac2f7dbce89c11fb/src/java.base/share/native/libjimage/imageFile.cpp#L59

    // Access bytes as unsigned.
    auto *bytes = reinterpret_cast<const unsigned char *>(str.data());
    auto useed = static_cast<uint32_t>(seed);

    // Compute hash code.
    for (auto byte = *bytes++; byte; byte = *bytes++)
    {
        useed = useed * util::HASH_MULTIPLIER ^ byte;
    }

    // Ensure the result is not signed.
    return static_cast<int32_t>(useed & 0x7FFFFFFF);
}


// Copied from: https://github.com/openjdk/jdk/blob/86800eb2b34bd6ea7a77e7a9ac2f7dbce89c11fb/src/java.base/share/native/libjimage/imageFile.cpp#L368
bool util::verify_location(const std::vector<unsigned char> &location_bytes,
        const std::vector<char> &strings, const uint32_t location_offset, const std::string &path)
{
    const auto offsets = parse_offsets(location_bytes, location_offset);

    // Position to first character of the path string.
    const char* next = path.c_str();

    // Get module name string.
    const char* module = get_string_at(strings, offsets.module);
    // If module string is not empty, compare '/module/'.
    if (*module != '\0')
    {
        if (*next++ != '/')
            return false;
        if (!((next = path_starts_with(next, module))))
            return false;
        if (*next++ != '/')
            return false;
    }

    // Get parent (package) string.
    const char* parent = get_string_at(strings, offsets.parent);
    // If parent string is not empty, compare 'parent/'.
    if (*parent != '\0')
    {
        if (!((next = path_starts_with(next, parent))))
            return false;
        if (*next++ != '/')
            return false;
    }

    // Get base name string and compare.
    const char* base = get_string_at(strings, offsets.base);
    if (!((next = path_starts_with(next, base))))
        return false;

    // Get extension string.
    const char* extension = get_string_at(strings, offsets.extension);
    // If extension is not empty, compare '.extension'.
    if (*extension != '\0')
    {
        if (*next++ != '.')
            return false;
        if (!((next = path_starts_with(next, extension))))
            return false;
    }

    // True only if complete match and no more characters.
    return *next == '\0';
}

static const char* path_starts_with(const char* str, const char* prefix)
{
    while (*prefix != '\0')
    {
        if (*str++ != *prefix++) return nullptr;
    }

    return str;
}

static const char* get_string_at(const std::vector<char> &strings, const uint64_t offset)
{
    return strings.data() + offset;
}

static location_string_offsets parse_offsets(
        const std::vector<unsigned char> &location_bytes, const uint32_t location_offset)
{
    location_string_offsets offsets{};

    size_t pos = location_offset;
    while (pos < location_bytes.size())
    {
        const uint8_t header = location_bytes[pos++];
        const uint8_t kind = header >> 3;
        const uint8_t value_size = (header & 0x7) + 1;

        if (kind == 0)
            break;

        uint64_t value = 0;
        for (uint8_t i = 0; i < value_size; ++i)
        {
            value = value << 8 | location_bytes[pos++];
        }

        switch (static_cast<jimage_location_attribute>(kind))
        {
        case jimage_location_attribute::MODULE:
            offsets.module = value;
            break;
        case jimage_location_attribute::PARENT:
            offsets.parent = value;
            break;
        case jimage_location_attribute::BASE:
            offsets.base = value;
            break;
        case jimage_location_attribute::EXTENSION:
            offsets.extension = value;
            break;

        default:
            break;
        }
    }

    return offsets;
}


uint64_t codesh::semantic_analyzer::external::util::read_location_attribute(
        const std::vector<unsigned char> &location_bytes, const uint32_t location_offset,
        const jimage_location_attribute kind)
{
    size_t pos = location_offset;
    while (pos < location_bytes.size())
    {
        const uint8_t header = location_bytes[pos++];
        const uint8_t k = header >> 3;
        const uint8_t value_size = (header & 0x7) + 1;

        if (k == 0)
            break;

        uint64_t value = 0;
        for (uint8_t i = 0; i < value_size; ++i)
        {
            value = value << 8 | location_bytes[pos++];
        }

        if (static_cast<jimage_location_attribute>(k) == kind)
            return value;
    }

    return 0;
}

// 100% AI slop below
std::vector<uint8_t> util::decompress_resource(const std::vector<uint8_t> &compressed,
        const uint64_t uncompressed_size, const std::vector<char> &strings)
{
    // JImage compressed block: 2-byte BE string-table offset of decompressor name, then payload
    const auto name_offset = static_cast<uint16_t>(compressed[0] << 8 | compressed[1]);
    const char *name = strings.data() + name_offset;
    if (std::string_view(name) != "zip")
        throw std::runtime_error(std::string("Unsupported JImage decompressor: ") + name);

    std::vector<uint8_t> result(uncompressed_size);
    auto dest_len = static_cast<uLongf>(uncompressed_size);
    const auto *src = compressed.data() + sizeof(uint16_t);
    const auto src_len = compressed.size() - sizeof(uint16_t);

    if (uncompress(result.data(), &dest_len, src, src_len) != Z_OK)
        throw std::runtime_error("JImage zlib decompression failed");

    return result;
}