#pragma once

#include <cstdint>
#include <iosfwd>
#include <string>

namespace codesh::semantic_analyzer::external::util
{

uint8_t read_u1(std::ifstream &file);
uint16_t read_u2(std::ifstream &file);
uint32_t read_u4(std::ifstream &file);

/**
 * @p seed is 0 for the first-level lookup
 *
 * The redirect table's value should be used as a second-level lookup
 */
uint32_t jimage_perfect_hash_index(const std::string &str, uint32_t table_length, uint32_t seed = 0);

}
