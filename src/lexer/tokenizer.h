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
[[nodiscard]] std::queue<std::unique_ptr<token>> tokenize_code(std::u32string code);
}