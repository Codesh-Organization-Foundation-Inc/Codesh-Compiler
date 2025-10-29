#pragma once

#include <optional>

namespace codesh
{
class token;
enum class token_group;
}


namespace codesh::definition
{

enum class visibility
{
    PUBLIC,
    PRIVATE,
    PROTECTED,
    PACKAGE_PRIVATE
};

std::optional<visibility> token_group_to_visibility(const token *token);

}