#pragma once

#include <deque>
#include <filesystem>
#include <unordered_map>

namespace codesh::lexer
{
struct code_position
{
    size_t line;
    size_t column;

    bool operator==(const code_position &other) const;
};

constexpr code_position NO_CODE_POS = {static_cast<size_t>(-1), static_cast<size_t>(-1)};
constexpr code_position FILE_BEGIN = {1, 0};

struct code_range
{
    code_position start;
    code_position end;
};


/**
 * Details relating to a keyword as it originally appeared in the original source code
 */
struct source_keyword_info
{
    size_t length;
};


struct code_position_hasher
{
    size_t operator()(const code_position &pos) const noexcept;
};

using code_pos_to_source_keyword_info = std::unordered_map<
    code_position,
    source_keyword_info,
    code_position_hasher
>;


struct source_file_info
{
    std::filesystem::path path;
    code_pos_to_source_keyword_info keyword_infos;
};


using global_source_info_map = std::deque<source_file_info>;

[[nodiscard]] global_source_info_map &get_global_source_info_map();

}
