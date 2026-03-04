#pragma once

#include "jimage_loader.h"

#include <cstdint>
#include <iosfwd>
#include <string>
#include <vector>

namespace codesh::semantic_analyzer::external::util
{

uint8_t read_u1(std::istream &file);
uint16_t read_u2(std::istream &file);
uint32_t read_u4(std::istream &file);

/*
 * Prime used to generate hash for Perfect Hashing.
 */
inline constexpr uint32_t HASH_MULTIPLIER = 0x01000193;

/**
 * @p seed is 0 for the first-level lookup
 *
 * The redirect table's value should be used as a second-level lookup
 */
int32_t jimage_perfect_hash_index(const std::string &str, uint32_t table_length, int32_t seed = HASH_MULTIPLIER);

/**
 * @returns True if the location encoded at @p location_offset in @p location_bytes matches @p path.
 */
[[nodiscard]] bool verify_location(const std::vector<unsigned char> &location_bytes,
        const std::vector<char> &strings, uint32_t location_offset, const std::string &path);

/**
 * @returns The value of attribute @p kind from the location entry at @p location_offset,
 * or 0 if the attribute is absent.
 */
[[nodiscard]] uint64_t read_location_attribute(const std::vector<unsigned char> &location_bytes,
        uint32_t location_offset, jimage_location_attribute kind);

[[nodiscard]] std::vector<uint8_t> decompress_resource(const std::vector<uint8_t> &compressed,
        uint64_t uncompressed_size, const std::vector<char> &strings);

}
