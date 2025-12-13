#pragma once

#include <string>
#include <vector>

namespace codesh::util
{

void replace_all(std::string& str, const std::string& from, const std::string& to);

void put_bytes(unsigned char arr[], const std::vector<unsigned char> &contents);

/**
 * Puts the number `num` into the array `arr` over `width` bytes.
 *
 * Uses big-endian order (as JVM uses it).
 */
void put_int_bytes(unsigned char arr[], size_t width, int num);

}