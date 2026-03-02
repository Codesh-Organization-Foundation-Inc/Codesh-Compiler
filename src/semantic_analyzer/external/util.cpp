#include "util.h"

#include <fstream>

uint32_t jimage_perfect_hash(const std::string &str, uint32_t seed);


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

uint32_t codesh::semantic_analyzer::external::util::jimage_perfect_hash_index(const std::string &str,
        const uint32_t table_length, const uint32_t seed)
{
    return jimage_perfect_hash(str, seed) % table_length;
}

uint32_t jimage_perfect_hash(const std::string &str, const uint32_t seed)
{
    uint32_t result = seed;

    for (const char chara : str)
    {
        result = result * 31 + chara;
    }

    return result;
}
