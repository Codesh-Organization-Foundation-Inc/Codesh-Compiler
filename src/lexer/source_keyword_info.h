#pragma once

#include "blasphemy/blasphemy_collector.h"

#include <unordered_map>

namespace codesh::lexer
{

/**
 * Details relating to a keyword as it originally appeared in the original source code
 */
struct source_keyword_info
{
    size_t length;
};


struct code_position_hasher
{
    size_t operator()(const blasphemy::code_position &pos) const noexcept;
};

using code_pos_to_source_keyword_info = std::unordered_map<
    blasphemy::code_position,
    source_keyword_info,
    code_position_hasher
>;
using file_id_to_code_pos_to_source_keyword_info = std::vector<code_pos_to_source_keyword_info>;

// shorten it lol
using global_source_info_map = file_id_to_code_pos_to_source_keyword_info;


global_source_info_map &get_global_source_info_map();

}
