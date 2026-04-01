#pragma once

#include <filesystem>
#include <memory>
#include <queue>
#include <string>

namespace codesh
{
class token;
}


namespace codesh::lexer
{
struct lexing_result
{
    std::queue<std::unique_ptr<token>> tokens;
    /**
     * A unique file ID to access an entry from @c get_global_source_info_map
     */
    size_t file_id = 0;
};

[[nodiscard]] lexing_result tokenize_code(std::filesystem::path path, const std::string &code);
[[nodiscard]] lexing_result tokenize_code(std::filesystem::path path, const std::u16string &code);
}