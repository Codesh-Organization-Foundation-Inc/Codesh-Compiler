#pragma once

#include <memory>
#include <queue>
#include <string>

namespace codesh
{
class token;
}


namespace codesh::lexer
{
[[nodiscard]] std::queue<std::unique_ptr<token>> tokenize_code(const std::string &code);
[[nodiscard]] std::queue<std::unique_ptr<token>> tokenize_code(const std::u16string &code);
}