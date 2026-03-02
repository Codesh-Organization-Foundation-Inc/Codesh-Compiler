#pragma once

#include <cstdint>
#include <iosfwd>

namespace codesh::semantic_analyzer::external::util
{

uint8_t read_u1(std::ifstream &file);
uint16_t read_u2(std::ifstream &file);
uint32_t read_u4(std::ifstream &file);

}
