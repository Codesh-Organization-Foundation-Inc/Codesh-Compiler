#include "visibility.h"

#include "../token/token.h"
#include "../token/token_group.h"

std::optional<codesh::definition::visibility> codesh::definition::token_group_to_visibility(
    const token *const token)
{
    switch (token->get_group())
    {
    case token_group::KEYWORD_PUBLIC: return visibility::PUBLIC;
    case token_group::KEYWORD_PRIVATE: return visibility::PRIVATE;
    case token_group::KEYWORD_PROTECTED: return visibility::PROTECTED;

    default: return std::nullopt;
    }
}
