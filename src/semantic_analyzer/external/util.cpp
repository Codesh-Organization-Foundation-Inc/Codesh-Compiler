#include "util.h"

#include <fstream>

int32_t jimage_perfect_hash(const std::string &str, uint32_t seed);

/*
 * Prime used to generate hash for Perfect Hashing.
 */
static constexpr uint32_t HASH_MULTIPLIER = 0x01000193;


uint8_t codesh::semantic_analyzer::external::util::read_u1(std::ifstream &file)
{
    uint8_t byte;
    file.read(reinterpret_cast<char *>(&byte), 1);
    return byte;
}

uint16_t codesh::semantic_analyzer::external::util::read_u2(std::ifstream &file)
{
    const uint16_t high = read_u1(file);
    const uint16_t low = read_u1(file);
    return static_cast<uint16_t>(high << 8 | low);
}

uint32_t codesh::semantic_analyzer::external::util::read_u4(std::ifstream &file)
{
    const uint32_t high = read_u2(file);
    const uint32_t low = read_u2(file);
    return high << 16 | low;
}

int32_t codesh::semantic_analyzer::external::util::jimage_perfect_hash_index(const std::string &str,
        const uint32_t table_length, const uint32_t seed)
{
    return jimage_perfect_hash(str, seed) % static_cast<int32_t>(table_length);
}

int32_t jimage_perfect_hash(const std::string &str, const uint32_t seed)
{
    // Copied from: https://github.com/openjdk/jdk/blob/86800eb2b34bd6ea7a77e7a9ac2f7dbce89c11fb/src/java.base/share/native/libjimage/imageFile.cpp#L59

    // Access bytes as unsigned.
    auto *bytes = reinterpret_cast<const unsigned char *>(str.data());
    uint32_t useed = seed;

    // Compute hash code.
    for (auto byte = *bytes++; byte; byte = *bytes++)
    {
        useed = useed * HASH_MULTIPLIER ^ byte;
    }

    // Ensure the result is not signed.
    return static_cast<int32_t>(useed & 0x7FFFFFFF);
}
