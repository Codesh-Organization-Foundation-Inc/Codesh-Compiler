#pragma once

#include <cstdint>
#include <iosfwd>

namespace codesh::semantic_analyzer::external::util
{

static uint8_t read_u1(std::ifstream &file);
static uint16_t read_u2(std::ifstream &file);
static uint32_t read_u4(std::ifstream &file);

}
